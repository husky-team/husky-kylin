#ifdef WITH_ORC
#include "husky-orc/io/input/orc_inputformat.hpp"

#include <sstream>
#include <string>

#include "boost/utility/string_ref.hpp"

#include "base/log.hpp"
#include "io/input/inputformat_helper.hpp"

namespace husky {
namespace io {

enum ORCInputFormatSetUp {
  NotSetUp = 0,
  InputSetUp = 1 << 1,
  AllSetUp = InputSetUp,
};

ORCInputFormat::ORCInputFormat() { is_setup_ = ORCInputFormatSetUp::NotSetUp; }
bool ORCInputFormat::is_setup() const {
  return !(is_setup_ ^ ORCInputFormatSetUp::AllSetUp);
}
void ORCInputFormat::set_input(const std::string& url) {
  if (!url_.empty() && url_ == url)
    // Setting with a same url last time will do nothing.
    return;
  url_ = url;
  int prefix = url_.find("://");
  ASSERT_MSG(prefix != std::string::npos,
             ("Cannot analyze protocol from " + url_).c_str());
  std::string protocol = url_.substr(0, prefix);
  splitter_.load(url_.substr(prefix + 3));
  is_setup_ |= ORCInputFormatSetUp::InputSetUp;
}
// buffer_ got from the orc_splitter must be '\n' seperated lines
// this saves us a lot of block handling
bool ORCInputFormat::next(boost::string_ref& ref) {
  if (buffer_.empty() || r == buffer_.size() - 1) {
    clear_buffer();
    bool success = fetch_new_block();
    if (success == false) {
      return false;
    }
  }
  r = helper::find_next(buffer_, l, '\n');
  ref = buffer_.substr(l, r - l);
  l = helper::find_next(buffer_, r, '\n') + 1;
  return true;
}
bool ORCInputFormat::fetch_new_block() {
  // fetch a new block
  buffer_ = splitter_.fetch_block();
  if (buffer_.empty()) {
    return false;
  }
  return true;
}
void ORCInputFormat::clear_buffer() {
  buffer_.clear();
  l = r = 0;
}
}  // namespace io
}  // namespace husky
#endif
