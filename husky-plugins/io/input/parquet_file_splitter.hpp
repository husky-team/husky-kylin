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

#pragma once

#ifdef WITH_PARQUET

#include <string>

#include "boost/utility/string_ref.hpp"
#include "parquet/file_reader.h"

#include "io/input/file_splitter_base.hpp"

namespace husky {
namespace io {

class PARQUETFileSplitter final : public FileSplitterBase {
 public:
  PARQUETFileSplitter();
  virtual ~PARQUETFileSplitter();

  // intialize the url of the PARQUET file
  // @param url starts with hdfs:// or nfs://
  virtual void load(std::string url);

  // ask master where to read from
  // @param is_next is false if need to ask master for a new block (same or
  // different files are possible)
  // ---------------is ture if can directly read the next block using the
  // current file
  boost::string_ref fetch_block(bool is_next = false) override;

  int read_block(const std::string& fn) override {}

  // get the starting position of splitter
  // just to keep consistent with LineInputFormat
  virtual size_t get_offset() { return offset_; }

 protected:
  void read_by_row(std::string fn);

  // url may be a directory or a file
  std::string url_;

  // row_batch_size lines of record
  std::string buffer;

  // starting position of reading
  int offset_;

  // block_size of one fetch_block operation
  // static size_t row_batch_size;

  // protocol can be hdfs or nfs
  std::string protocol_;

  // current filename
  std::string cur_fn;

  // PARQUET reader to help to read PARQUET files
  std::unique_ptr<parquet::ParquetFileReader> reader;
};

}  // namespace husky
}  // namespace io

#endif
