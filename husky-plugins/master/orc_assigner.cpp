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

namespace husky {

using orc::Reader;
using orc::ReaderOptions;
using orc::createReader;
using orc::readLocalFile;

static ORCBlockAssigner orc_block_assigner;

ORCBlockAssigner::ORCBlockAssigner() {
  Master::get_instance().register_main_handler(
      TYPE_ORC_BLK_REQ,
      std::bind(&ORCBlockAssigner::master_main_handler, this));
  Master::get_instance().register_setup_handler(
      std::bind(&ORCBlockAssigner::master_setup_handler, this));
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
  num_workers_alive_ = Context::get_worker_info().get_num_workers();
}

void ORCBlockAssigner::browse_local(const std::string& url) {
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
}

std::pair<std::string, size_t> ORCBlockAssigner::answer(std::string& url) {
  // Directory or file status initialization
  // This condition is true either when the begining of the file or
  // all the workers has finished reading this file or directory
  if (finish_dict_.find(url) == finish_dict_.end()) browse_local(url);
  std::pair<std::string, size_t> ret = {"", 0};  // selected_file, offset
  if (boost::filesystem::is_regular_file(url)) {
    if (file_offset_[url] < file_size_[url]) {
      ret.first = url;
      ret.second = file_offset_[url];
      file_offset_[url] += row_batch_size_;
    }
  } else if (boost::filesystem::is_directory(url)) {
    for (auto files : boost::filesystem::directory_iterator(url)) {
      std::string path = files.path().string();
      // if this file hasn't been finished
      if (finish_dict_.find(path) != finish_dict_.end()) {
        if (file_offset_[path] < file_size_[path]) {
          ret.first = path;
          ret.second = file_offset_[path];
          file_offset_[path] += row_batch_size_;
          // no need to continue searching for next file
          break;
        } else {
          finish_dict_[path] += 1;
          if (finish_dict_[path] == num_workers_alive_) {
            finish_url(path);
          }
          // need to search for next file
          continue;
        }
      }
    }
  }
  // count how many workers won't ask for an answer
  if (ret.first == "" && ret.second == 0) {
    finish_dict_[url] += 1;
    if (finish_dict_[url] == num_workers_alive_) {
      finish_url(url);
    }
  }
  // Once ret hasn't been assigned value, answer(url) will not be called
  // anymore.
  // Reason:
  // ret = {"", 0} => splitter: fetch_block return "" => inputformat: next
  // return false =>
  // executor: load break loop
  return ret;
}

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

}  // namespace husky
#endif
