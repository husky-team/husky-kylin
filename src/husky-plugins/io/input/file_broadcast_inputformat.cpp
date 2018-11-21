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

#include "husky-plugins/io/input/file_broadcast_inputformat.hpp"

#include <algorithm>
#include <string>

#include "boost/utility/string_ref.hpp"
#include "hdfs/hdfs.h"

#include "core/context.hpp"

namespace husky {
namespace io {

void FileBroadcastInputFormat::set_input(const std::string& url) {
    int prefix = url.find("://");
    ASSERT_MSG(prefix != std::string::npos, ("Cannot analyze protocol from " + url).c_str());
    std::string protocol = url.substr(0, prefix);
    ASSERT_MSG(protocol == "hdfs", "Now support hdfs only");
    url_ = url.substr(prefix + 3);

    if (fs_ == nullptr) {
        // connect to hdfs
        hdfsBuilder* builder = hdfsNewBuilder();
        hdfsBuilderSetNameNode(builder, husky::Context::get_param("hdfs_namenode").c_str());
        hdfsBuilderSetNameNodePort(builder, std::stoi(husky::Context::get_param("hdfs_namenode_port")));
        fs_ = hdfsBuilderConnect(builder);
        if (fs_ == nullptr) {
            husky::LOG_E << "Cannot connect to hdfs";
        }
    }

    int num_files = 0;
    auto file_info = hdfsListDirectory(fs_, url_.c_str(), &num_files);
    if (num_files == 0) {
        husky::LOG_E << "Cannot ls " << url_;
    }
    size_t max_size = 0;
    for (int i = 0; i < num_files; ++i) {
        if (file_info[i].mKind == kObjectKindFile) {
            file_sizes_.insert({std::string(file_info[i].mName), file_info[i].mSize});
            max_size = std::max(max_size, (size_t) file_info[i].mSize);
        }
    }
    data_.reset(new char[max_size], [](char* p) { delete[] p; });
    hdfsFreeFileInfo(file_info, num_files);
}

FileBroadcastInputFormat::~FileBroadcastInputFormat() {
    if (file_) {
        hdfsCloseFile(fs_, file_);
    }
    if (fs_) {
        hdfsDisconnect(fs_);
    }
}

bool FileBroadcastInputFormat::next(boost::string_ref& ref) {
    if (file_sizes_.empty()) {
        return false;
    }
    auto pair = file_sizes_.begin();
    auto size = fetch_file(pair->first, pair->second);
    file_sizes_.erase(pair);
    ref = boost::string_ref(data_.get(), size);
    return size != 0;
}

size_t FileBroadcastInputFormat::fetch_file(const std::string& file_name, size_t size) {
    if (file_ != nullptr) {
        hdfsCloseFile(fs_, file_);
        file_ = nullptr;
    }

    file_ = hdfsOpenFile(fs_, file_name.c_str(), O_RDONLY, 0, 0, 0);
    hdfsSeek(fs_, file_, 0);
    ASSERT_MSG(file_ != nullptr, "Cannot open file " + file_name);
    size_t start = 0;
    size_t nbytes = 0;
    while (start < size) {
        nbytes = hdfsRead(fs_, file_, data_.get() + start, size);
        start += nbytes;
        if (nbytes == 0) {
            break;
        }
    }
    return start;
}

}  // namespace io
}  // namespace husky
