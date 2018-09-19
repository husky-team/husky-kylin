#ifdef WITH_ORC
#include "husky-orc/io/input/orc_file_splitter.hpp"

#include <string>

#include "orc/ColumnPrinter.hh"
#include "orc/OrcFile.hh"

#include "base/log.hpp"
#include "base/serialization.hpp"
#include "core/constants.hpp"
#include "core/context.hpp"
#include "core/coordinator.hpp"

#include "husky-orc/core/constants.hpp"

namespace husky {
namespace io {

using orc::ReaderOptions;
using orc::createReader;
using orc::readLocalFile;
using orc::ColumnVectorBatch;

// default number of lines in one read operation
size_t ORCFileSplitter::row_batch_size = 8 * 1024;
ORCFileSplitter::ORCFileSplitter() { offset_ = 0; }
ORCFileSplitter::~ORCFileSplitter() {}
// initialize reader with the file url
// TODO:
// 1. add orcReaderOptions
// 2. readHdfsFile(url)
void ORCFileSplitter::load(std::string url) {
  cur_fn = "";
  url_ = url;
  protocol_ = "nfs";
}
// ask master for offset and url
// we are not using fn for now since url is not a directory
boost::string_ref ORCFileSplitter::fetch_block(bool is_next) {
  BinStream question;
  question << url_;
  BinStream answer = husky::Context::get_coordinator()->ask_master(
      question, husky::TYPE_ORC_BLK_REQ);
  std::string fn;
  answer >> fn;
  answer >> offset_;
  if (fn == "") {
    return "";
  } else if (fn != cur_fn) {
    cur_fn = fn;
    ReaderOptions opts;
    reader = createReader(readLocalFile(cur_fn), opts);
    int rows_per_stripe = reader->getStripe(0)->getNumberOfRows();
    if (rows_per_stripe != 0) {
      row_batch_size = rows_per_stripe;
    }
  }
  read_by_row(fn);
  return boost::string_ref(buffer);
}
void ORCFileSplitter::read_by_row(std::string fn) {
  if (protocol_ == "hdfs") {
    return;
    // to be done
  } else if (protocol_ == "nfs") {
    try {
      std::string line = "";

      orc::RowReaderOptions row_opts;
      ORC_UNIQUE_PTR<orc::RowReader> rowReader =
          reader->createRowReader(row_opts);
      rowReader->seekToRow(offset_);
      std::unique_ptr<orc::ColumnPrinter> printer =
          createColumnPrinter(line, &rowReader->getSelectedType());
      std::unique_ptr<ColumnVectorBatch> batch =
          rowReader->createRowBatch(row_batch_size);
      if (!buffer.empty()) {
        buffer.clear();
      }
      if (rowReader->next(*batch)) {
        printer->reset(*batch);
        unsigned long i;
        for (i = 0; i < batch->numElements; ++i) {
          line.clear();
          printer->printRow(i);
          line += "\n";
          buffer += line;
        }
      }
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
