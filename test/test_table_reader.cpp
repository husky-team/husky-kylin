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

#include <algorithm>
#include <string>
#include <vector>

#include "table/text_table_reader.hpp"

#include "base/log.hpp"
#include "core/engine.hpp"

using husky::table::TextTableReader;

void execute() {
    TextTableReader reader;
    reader.read_data(husky::Context::get_param("input"));
    auto raw_table = reader.get_data();
    husky::LOG_I << "raw_table row count " << raw_table.size();
    husky::LOG_I << "raw_table column count " << reader.get_column_num();
    size_t print_n = std::min((size_t) 10, raw_table.size());
    for (size_t i = 0; i < print_n; ++i) {
        std::stringstream ss;
        for (auto& cell : raw_table[i]) {
            ss << cell << "\t";
        }
        husky::LOG_I << ss.str();
    }
}

int main(int argc, char** argv) {
    std::vector<std::string> args;
    args.push_back("hdfs_namenode");
    args.push_back("hdfs_namenode_port");
    args.push_back("input");

    if (husky::init_with_args(argc, argv, args)) {
        husky::run_job(execute);
        return 0;
    }

    return 1;
}
