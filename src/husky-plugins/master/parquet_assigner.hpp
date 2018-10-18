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

#include <map>
#include <string>
#include <utility>

#include "hdfs/hdfs.h"
#include "parquet/file_reader.h"

#include "master/master.hpp"

namespace husky {

class PARQUETBlockAssigner {
   public:
    PARQUETBlockAssigner();
    ~PARQUETBlockAssigner() = default;

    void master_main_handler();
    void master_setup_handler();
    void browse_local(const std::string& url);
    std::pair<std::string, size_t> answer(std::string& url);
    std::pair<std::string, size_t> answer_hdfs(std::string& url);
    /// Return the number of workers who have finished reading the files in
    /// the given url
    int get_num_finished(std::string& url);
    /// Use this when all workers finish reading the files in url
    void finish_url(std::string& url);
    void init_hdfs(const std::string& node, const std::string& port);
    void browse_hdfs(const std::string& url);

   private:
    // int row_batch_size = 8*1024;
    int num_workers_alive_;
    std::map<std::string, size_t> file_offset_;
    std::map<std::string, size_t> file_size_;
    std::map<std::string, int> finish_dict_;
    std::unique_ptr<parquet::ParquetFileReader> reader_;
    std::string protocol_;
    // handle of HDFS
    hdfsFS fs_;
};

}  // namespace husky

#endif
