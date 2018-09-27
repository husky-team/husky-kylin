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

#include "husky-plugins/io/input/parquet_file_splitter.hpp"

#include <string>

#include "parquet/column_reader.h"
#include "parquet/column_scanner.h"
#include "parquet/file_reader.h"
#include "parquet/metadata.h"
#include "parquet/printer.h"

#include "base/log.hpp"
#include "base/serialization.hpp"
#include "core/context.hpp"
#include "core/coordinator.hpp"

#include "husky-plugins/core/constants.hpp"

namespace husky {
namespace io {

// default number of lines in one read operation
// size_t PARQUETFileSplitter::row_batch_size = 8*1024;

PARQUETFileSplitter::PARQUETFileSplitter() { offset_ = 0; }

PARQUETFileSplitter::~PARQUETFileSplitter() {}
// initialize reader with the file url
// TODO:
// 1. add PARQUETReaderOptions
// 2. readHdfsFile(url)
void PARQUETFileSplitter::load(std::string url) {
  cur_fn_ = "";
  url_ = url;
  protocol_ = "nfs";
}

// ask master for offset and url
// we are not using fn for now since url is not a directory
boost::string_ref PARQUETFileSplitter::fetch_block(bool is_next) {
  BinStream question;
  question << url_;
  BinStream answer = husky::Context::get_coordinator()->ask_master(
      question, husky::TYPE_PARQUET_BLK_REQ);
  std::string fn;
  answer >> fn;
  answer >> offset_;
  if (fn == "") {
    return "";
  } else if (fn != cur_fn_) {
    cur_fn_ = fn;
    bool memory_map = true;
    reader_ = parquet::ParquetFileReader::OpenFile(cur_fn_, memory_map);
  }
  read_by_row(fn);
  return boost::string_ref(buffer_);
}

void PARQUETFileSplitter::read_by_row(std::string fn) {
  if (protocol_ == "hdfs") {
    return;
    // to be done
  } else if (protocol_ == "nfs") {
    try {
      std::string line = "";
      parquet::ParquetFilePrinter printer(reader_.get());
      const parquet::FileMetaData* file_metadata = reader_->metadata().get();
      auto group_reader = reader_->RowGroup(offset_);
      // Create readers for selected columns and print contents
      std::vector<std::shared_ptr<parquet::Scanner>> scanners(
          file_metadata->num_columns(), nullptr);
      for (int i = 0; i < file_metadata->num_columns(); i++) {
        std::shared_ptr<parquet::ColumnReader> col_reader =
            group_reader->Column(i);
        // This is OK in this method as long as the RowGroupReader does not get
        // deleted
        scanners[i] = parquet::Scanner::Make(ColReader);
      }
      bool hasRow;
      do {
        hasRow = false;
        for (auto scanner : scanners) {
          if (scanner->HasNext()) {
            std::stringstream stream;
            hasRow = true;
            scanner->PrintNext(stream, 27);
            buffer_ += stream.str();
          }
        }
        buffer_ += "\n";
      } while (hasRow);

    } catch (const std::exception& e) {
      base::log_msg(e.what());
    }
  } else {
    base::log_msg("Protocol:" + protocol_ + " is not suported");
    return;
  }
}

}  // namespace io
}  // namespace husky

#endif
