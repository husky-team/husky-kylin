#ifdef WITH_ORC
#include "husky-plugins/master/orc_assigner.hpp"

#include <fstream>
#include <string>
#include <utility>

#include "boost/filesystem.hpp"
#include "orc/Reader.hh"

#include "base/log.hpp"
#include "core/constants.hpp"
#include "core/context.hpp"
#include "master/master.hpp"

#include "husky-plugins/core/constants.hpp"
#include "husky-plugins/io/input/orc_hdfs_inputstream.hpp"

namespace husky {

using orc::Reader;
using orc::ReaderOptions;
using orc::createReader;
using orc::readLocalFile;
using orc::ColumnVectorBatch;

static ORCBlockAssigner orc_block_assigner;

ORCBlockAssigner::ORCBlockAssigner() {
    Master::get_instance().register_main_handler(TYPE_ORC_BLK_REQ,
                                                 std::bind(&ORCBlockAssigner::master_main_handler, this));
    Master::get_instance().register_setup_handler(std::bind(&ORCBlockAssigner::master_setup_handler, this));
}

void ORCBlockAssigner::master_main_handler() {
    auto& master = Master::get_instance();
    auto resp_socket = master.get_socket();
    std::string url;
    BinStream stream = zmq_recv_binstream(resp_socket.get());
    stream >> url;
    std::pair<std::string, size_t> ret = answer(url);
    stream.clear();
    stream << ret.first << ret.second;
    zmq_sendmore_string(resp_socket.get(), master.get_cur_client());
    zmq_sendmore_dummy(resp_socket.get());
    zmq_send_binstream(resp_socket.get(), stream);
    base::log_msg(" => " + ret.first + "@" + std::to_string(ret.second));
}

void ORCBlockAssigner::master_setup_handler() {
    init_hdfs(Context::get_param("hdfs_namenode"), Context::get_param("hdfs_namenode_port"));
    num_workers_alive_ = Context::get_worker_info().get_num_workers();
}

/* void ORCBlockAssigner::browse_local(const std::string& url) {
  // If url is a directory, recursively traverse all files in url
  // TODO(yidi) here I assume that url is a valid file
  try {
    if (boost::filesystem::exists(url)) {
      if (boost::filesystem::is_regular_file(url)) {
        std::unique_ptr<Reader> reader;
        ReaderOptions opts;
        reader = createReader(readLocalFile(url), opts);
        int rows_per_stripe = reader->getStripe(0)->getNumberOfRows();
        if (rows_per_stripe != 0) {
          row_batch_size_ = rows_per_stripe;
        }
        file_size_[url] = reader->getNumberOfRows();
        file_offset_[url] = 0;
        finish_dict_[url] = 0;
      } else if (boost::filesystem::is_directory(url)) {
        // url should also be included in the finish_dict_
        finish_dict_[url] = 0;
        for (auto files : boost::filesystem::directory_iterator(url)) {
          std::string path = files.path().string();
          if (boost::filesystem::is_regular_file(path)) {
            std::unique_ptr<Reader> reader;
            ReaderOptions opts;
            reader = createReader(readLocalFile(path), opts);
            int rows_per_stripe = reader->getStripe(0)->getNumberOfRows();
            file_size_[path] = reader->getNumberOfRows();
            file_offset_[path] = 0;
            finish_dict_[path] = 0;
          }
        }
      } else {
        base::log_msg("Given url:" + url +
                      " is not a regular file or diercotry");
      }
    } else {
      base::log_msg("Given url:" + url + " doesn't exist!");
    }
  } catch (const std::exception& ex) {
    base::log_msg("Exception cought: ");
    base::log_msg(ex.what());
  }
} */

void ORCBlockAssigner::init_hdfs(const std::string& node, const std::string& port) {
    int num_retries = 3;
    while (num_retries--) {
        struct hdfsBuilder* builder = hdfsNewBuilder();
        hdfsBuilderSetNameNode(builder, node.c_str());
        hdfsBuilderSetNameNodePort(builder, std::stoi(port));
        fs_ = hdfsBuilderConnect(builder);
        hdfsFreeBuilder(builder);
        if (fs_)
            break;
    }
    if (fs_)
        return;
    else
        LOG_I << "Failed to connect to HDFS " << node << ":" << port;
}

void ORCBlockAssigner::browse_hdfs(const std::string& url) {
    if (!fs_)
        return;

    try {
        int num_files;
        size_t total = 0;
        auto& files = files_dict_[url];
        hdfsFileInfo* file_info = hdfsListDirectory(fs_, url.c_str(), &num_files);
        LOG_I << "Total number of files: " << num_files;
        for (int i = 0; i < num_files; i++) {
            // for each files in the dir
            if (file_info[i].mKind != kObjectKindFile)
                continue;
            ReaderOptions opts;
            reader_ = createReader(io::read_hdfs_file(fs_, file_info[i].mName), opts);
            size_t num_rows = reader_->getNumberOfRows();
            files.push_back(OrcFileDesc{std::string(file_info[i].mName) + '\0', num_rows, 0});
            total += num_rows;
        }
        LOG_I << "Total number of rows: " << total;
        hdfsFreeFileInfo(file_info, num_files);
    } catch (const std::exception& ex) {
        LOG_I << "Exception Caught: " << ex.what();
    }
}

std::pair<std::string, size_t> ORCBlockAssigner::answer(std::string& url) {
    // Directory or file status initialization
    // This condition is true either when the begining of the file or
    // all the workers has finished reading this file or directory
    if (!fs_)
        return {"", 0};

    if (finish_dict_.find(url) == finish_dict_.end()) {
        browse_hdfs(url);
        finish_dict_[url];
    }

    auto& files = files_dict_[url];
    if (files.empty()) {
        finish_dict_[url] += 1;
        if (finish_dict_[url] == num_workers_alive_) {
            files_dict_.erase(url);
        }
        return {"", 0};
    }

    auto& file = files.back();
    std::pair<std::string, size_t> ret = {file.filename, file.offset};
    file.offset += io::kOrcRowBatchSize;

    if (file.offset > file.num_of_rows) {
        files.pop_back();
    }
    return ret;
}

/*
/// Return the number of workers who have finished reading the files in
/// the given url
int ORCBlockAssigner::get_num_finished(std::string& url) {
  return finish_dict_[url];
}

/// Use this when all workers finish reading the files in url
void ORCBlockAssigner::finish_url(std::string& url) {
  file_size_.erase(url);
  file_offset_.erase(url);
  finish_dict_.erase(url);
}
*/

}  // namespace husky
#endif
