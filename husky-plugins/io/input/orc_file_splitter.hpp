#ifdef WITH_ORC
#pragma once

#include <string>

#include "boost/utility/string_ref.hpp"
#include "orc/ColumnPrinter.hh"
#include "orc/orc-config.hh"

#include "io/input/file_splitter_base.hpp"

namespace husky {
namespace io {

class ORCFileSplitter final : public FileSplitterBase {
 public:
  ORCFileSplitter();
  virtual ~ORCFileSplitter();
  // intialize the url of the orc file
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
  inline size_t get_offset() { return offset_; }

 protected:
  void read_by_row(std::string fn);
  // url may be a directory or a file
  std::string url_;
  // row_batch_size lines of record
  std::string buffer;
  // starting position of reading
  size_t offset_;
  // block_size of one fetch_block operation
  static size_t row_batch_size;
  // protocol can be hdfs or nfs
  std::string protocol_;
  // current filename
  std::string cur_fn;
  // orc reader to help to read orc files
  std::unique_ptr<orc::Reader> reader;
};
}  // namespace husky
}  // namespace io
#endif
