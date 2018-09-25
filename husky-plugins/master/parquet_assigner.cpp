// Copyright 2018 Husky Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef WITH_PARQUET

#include <fstream>
#include <string>
#include <utility>

#include "boost/filesystem.hpp"
#include "parquet/column_reader.h"
#include "parquet/file_reader.h"
#include "parquet/metadata.h"

#include "base/log.hpp"
#include "core/context.hpp"
#include "master/master.hpp"

#include "husky-plugins/master/parquet_assigner.hpp"
#include "husky-plugins/core/constants.hpp"

namespace husky {

static PARQUETBlockAssigner PARQUET_block_assigner;

PARQUETBlockAssigner::PARQUETBlockAssigner() {
  Master::get_instance().register_main_handler(
      TYPE_PARQUET_BLK_REQ,
      std::bind(&PARQUETBlockAssigner::master_main_handler, this));
  Master::get_instance().register_setup_handler(
      std::bind(&PARQUETBlockAssigner::master_setup_handler, this));
}

void PARQUETBlockAssigner::master_main_handler() {
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

void PARQUETBlockAssigner::master_setup_handler() {
  num_workers_alive = Context::get_worker_info().get_num_workers();
}

void PARQUETBlockAssigner::browse_local(const std::string& url) {
  // If url is a directory, recursively traverse all files in url
  try {
    if (boost::filesystem::exists(url)) {
      if (boost::filesystem::is_regular_file(url)) {
        reader_ = parquet::ParquetFileReader::OpenFile(url, true);
        const parquet::FileMetaData* FileMetadata = reader_->metadata().get();
        file_size_[url] = FileMetadata->num_row_groups();
        file_offset_[url] = 0;
        finish_dict_[url] = 0;
      } else if (boost::filesystem::is_directory(url)) {
        // url should also be included in the finish_dict
        finish_dict_[url] = 0;
        for (auto files : boost::filesystem::directory_iterator(url)) {
          std::string path = files.path().string();
          if (boost::filesystem::is_regular_file(path)) {
            reader_ = parquet::ParquetFileReader::OpenFile(url, true);
            const parquet::FileMetaData* FileMetadata =
                reader_->metadata().get();
            file_size_[url] = FileMetadata->num_row_groups();
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

std::pair<std::string, size_t> PARQUETBlockAssigner::answer(std::string& url) {
  // Directory or file status initialization
  // This condition is true either when the begining of the file or
  // all the workers has finished reading this file or directory
  if (finish_dict_.find(url) == finish_dict_.end()) browse_local(url);
  std::pair<std::string, size_t> ret = {"", 0};  // selected_file, offset
  if (boost::filesystem::is_regular_file(url)) {
    if (file_offset_[url] < file_size_[url]) {
      ret.first = url;
      ret.second = file_offset_[url];
      file_offset_[url] += 1;
    }
  } else if (boost::filesystem::is_directory(url)) {
    for (auto files : boost::filesystem::directory_iterator(url)) {
      std::string path = files.path().string();
      // if this file hasn't been finished
      if (finish_dict_.find(path) != finish_dict_.end()) {
        if (file_offset_[path] < file_size_[path]) {
          ret.first = path;
          ret.second = file_offset_[path];
          file_offset_[path] += 1;
          // no need to continue searching for next file
          break;
        } else {
          finish_dict_[path] += 1;
          if (finish_dict_[path] == num_workers_alive_) {
            finish_url_(path);
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
      finish_url_(url);
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
int PARQUETBlockAssigner::get_num_finished(std::string& url) {
  return finish_dict_[url];
}

/// Use this when all workers finish reading the files in url
void PARQUETBlockAssigner::finish_url(std::string& url) {
  file_size_.erase(url);
  file_offset_.erase(url);
  finish_dict_.erase(url);
}

}  // namespace husky
#endif
