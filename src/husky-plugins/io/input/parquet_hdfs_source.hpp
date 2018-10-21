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
#pragma once

#include <mutex>
#include <string>

#include "arrow/buffer.h"
#include "hdfs/hdfs.h"
#include "parquet/util/memory.h"

#include "base/assert.hpp"
#include "base/exception.hpp"
#include "base/log.hpp"
#include "base/thread_support.hpp"

namespace husky {
namespace io {

using husky::base::HuskyException;

class PARQUETHdfsSource final : public parquet::RandomAccessSource {
   public:
    PARQUETHdfsSource(hdfsFS hdfs_fs, const std::string& file) {
        hdfs_fs_ = hdfs_fs;
        file_name_ = file;
        hdfs_file_ = hdfsOpenFile(hdfs_fs_, file_name_.c_str(), O_RDONLY, 0, 0, 0);
        ASSERT_MSG(hdfs_file_ != NULL, "hdfsOpenFile failed");
        hdfsFileInfo* file_info = hdfsGetPathInfo(hdfs_fs_, file_name_.c_str());
        length_ = file_info->mSize;
        hdfsFreeFileInfo(file_info, 1);
    }

    ~PARQUETHdfsSource() { hdfsCloseFile(hdfs_fs_, hdfs_file_); }

    int64_t Size() const override { return static_cast<int64_t>(length_); };

    // Returns bytes read
    // Called by MetaDataReader
    int64_t ReadAt(int64_t position, int64_t length, uint8_t* out) override {
        if (!out)
            throw HuskyException("Buffer is null");

        hdfsSeek(hdfs_fs_, hdfs_file_, position);
        int32_t remain = static_cast<int32_t>(length);
        int32_t start = 0;
        int32_t nbytes = 0;
        out = static_cast<unsigned char*>(out);
        while (remain > 0) {
            nbytes = hdfsRead(hdfs_fs_, hdfs_file_, out + start, remain);
            start += nbytes;
            remain -= nbytes;
        }
        if (start == -1)
            throw HuskyException("Bad read of " + file_name_);
        if (static_cast<int64_t>(start) != length)
            throw HuskyException("Short read of " + file_name_);
        return length;
    };

    int64_t Read(int64_t nbytes, uint8_t* out) override { return ReadAt(Size(), nbytes, out); };

    // Called by Column(i)
    std::shared_ptr<arrow::Buffer> ReadAt(int64_t position, int64_t nbytes) override {
        const int32_t read_size = 128 * 1024;
        uint8_t* out = (uint8_t*) malloc(nbytes * sizeof(uint8_t));
        hdfsSeek(hdfs_fs_, hdfs_file_, position);
        int32_t remain = static_cast<int32_t>(nbytes);
        int32_t start = 0;
        int32_t nreadbytes = 0;
        while (remain > 0) {
            // only 128KB per hdfsRead
            if (remain < read_size) {
                const int32_t remain_size = remain;
                uint8_t buffer[remain_size];
                nreadbytes = hdfsRead(hdfs_fs_, hdfs_file_, (unsigned char*) buffer, remain_size);
                for (int32_t i = 0; i < nreadbytes; i++)
                    out[i + start] = buffer[i];
                start += nreadbytes;
                remain -= nreadbytes;
                if (remain > 0)
                    continue;
                return std::shared_ptr<arrow::Buffer>(new arrow::Buffer(out, nbytes));
            }
            uint8_t buf[read_size];
            nreadbytes = hdfsRead(hdfs_fs_, hdfs_file_, (unsigned char*) buf, read_size);
            for (int32_t i = 0; i < nreadbytes; i++)
                out[i + start] = buf[i];
            start += nreadbytes;
            remain -= nreadbytes;
        }
        return std::shared_ptr<arrow::Buffer>(new arrow::Buffer(out, nbytes));
    };

    std::shared_ptr<arrow::Buffer> Read(int64_t nbytes) override {
        uint8_t* out;
        if (nbytes == Read(nbytes, out)) {
            return std::shared_ptr<arrow::Buffer>(new arrow::Buffer(out, Size()));
        }
    };

    void Close() override{};
    int64_t Tell() override { return 0; };

   private:
    std::string file_name_;
    hdfsFile hdfs_file_;
    hdfsFS hdfs_fs_;
    int64_t length_;
    std::mutex read_mutex;
};

std::unique_ptr<parquet::RandomAccessSource> hdfs_source(hdfsFS hdfs_fs, const std::string& path) {
    return std::unique_ptr<parquet::RandomAccessSource>(new PARQUETHdfsSource(hdfs_fs, path));
}

}  // namespace io
}  // namespace husky

#endif
