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

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "base/log.hpp"
#include "core/engine.hpp"

#include "husky-plugins/io/input/file_broadcast_inputformat.hpp"

using JSON = nlohmann::json;

void execute() {
    // set chunk size to max
    husky::io::FileBroadcastInputFormat infmt;
    infmt.set_input(husky::Context::get_param("input"));
    auto parse_json = [&](boost::string_ref& chunk) { husky::LOG_I << JSON::parse(chunk).dump(1, ' ', true); };
    husky::load(infmt, parse_json);
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
