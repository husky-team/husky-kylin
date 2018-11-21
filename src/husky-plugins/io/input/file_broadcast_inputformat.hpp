// Copyright 2016 Husky Team
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

#include <string>
#include <unordered_map>

#include "boost/utility/string_ref.hpp"
#include "hdfs/hdfs.h"

#include "io/input/inputformat_base.hpp"

namespace husky {
namespace io {

class FileBroadcastInputFormat : public InputFormatBase {
   public:
    typedef boost::string_ref RecordT;

    virtual ~FileBroadcastInputFormat();

    void set_input(const std::string& url);
    bool next(boost::string_ref& ref);
    void clear_buffer() { data_.reset(); }
    bool is_setup() const override { return true; }

   protected:
    size_t fetch_file(const std::string& file_name, size_t size);

   private:
    std::string url_;

    hdfsFS fs_ = nullptr;
    hdfsFile file_ = nullptr;
    std::shared_ptr<char> data_ = nullptr;
    std::unordered_map<std::string, size_t> file_sizes_;
};

}  // namespace io
}  // namespace husky
