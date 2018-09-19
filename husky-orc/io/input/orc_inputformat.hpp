#ifdef WITH_ORC
#pragma once

#include <cassert>
#include <fstream>
#include <string>

#include "boost/utility/string_ref.hpp"

#include "io/input/inputformat_base.hpp"

#include "husky-orc/io/input/orc_file_splitter.hpp"

namespace husky {
namespace io {

class ORCInputFormat : public InputFormatBase {
 public:
  typedef boost::string_ref RecordT;
  ORCInputFormat();
  virtual ~ORCInputFormat() = default;
  virtual void set_input(const std::string& url);
  virtual bool next(boost::string_ref& ref);
  virtual bool is_setup() const;

 protected:
  bool fetch_new_block();
  void clear_buffer();
  std::string url_;
  int l = 0;
  int r = 0;
  boost::string_ref buffer_;
  ORCFileSplitter splitter_;
};

}  // namespace io
}  // namespace husky
#endif
