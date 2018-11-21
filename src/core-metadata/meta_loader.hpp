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

#include <map>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "core/engine.hpp"

#include "husky-plugins/io/input/file_broadcast_inputformat.hpp"

namespace husky {
namespace cube {

using nlohmann::json;

class MetaLoader {
   public:
    const std::string CUBE_DESC = "/cube_desc";
    const std::string MODEL_DESC = "/model_desc";
    const std::string TABLE = "/table";

    explicit MetaLoader(const std::string meta_url) : meta_url_(meta_url) {
        auto pos = meta_url_.find("@hdfs");
        ASSERT_MSG(pos != meta_url_.npos, "the meta url should look like @hdfs");
        pos = meta_url_.find("path=");
        auto url = meta_url_.substr(pos + 5);
        std::string protocol = "hdfs://";
        pos = url.find("/", url.find(protocol) + protocol.size());
        hdfs_dir_ = protocol + url.substr(pos);
        husky::LOG_I << hdfs_dir_;
    }

    json get_cube_desc() {
        auto jsons = get_json(CUBE_DESC);
        return jsons.empty() ? json() : jsons.front();
    }

    json get_model_desc() {
        auto jsons = get_json(MODEL_DESC);
        return jsons.empty() ? json() : jsons.front();
    }

    std::map<std::string, std::shared_ptr<json>> get_tables() {
        std::map<std::string, std::shared_ptr<json>> ret;
        auto tables = get_json(TABLE);
        for (auto& table : tables) {
            ret.insert({table["database"].get<std::string>() + "." + table["name"].get<std::string>(),
                        std::make_shared<json>(std::move(table))});
        }
        return ret;
    }

    json get_json(const std::string& suffix) {
        std::vector<json> ret;
        infmt_.set_input(hdfs_dir_ + suffix);
        auto parse_json = [&ret](boost::string_ref& chunk) {
            auto start_pos = chunk.find("{");
            auto end_pos = chunk.rfind("}");
            ret.push_back(json::parse(chunk.substr(start_pos, end_pos - start_pos + 1)));
        };
        husky::load(infmt_, parse_json);
        return ret;
    }

   private:
    std::string meta_url_;
    std::string hdfs_dir_;
    io::FileBroadcastInputFormat infmt_;
};

}  // namespace cube
}  // namespace husky
