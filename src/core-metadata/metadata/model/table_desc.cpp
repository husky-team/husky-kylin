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

#include "core-metadata/metadata/model/table_desc.hpp"

#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

#include "core-metadata/metadata/model/column_desc.hpp"

namespace husky {
namespace cube {

using json = nlohmann::json;

TableDesc::TableDesc(const std::string& table_json_path) {
    // std::string table_json_path = "table.json"; // should be in hdfs
    std::ifstream ifs(table_json_path);
    json j = json::parse(ifs);

    // for now, only support a fact table
    name_ = j["name"].get<std::string>();
    json j_columns = j["columns"];
    for (json::iterator it = j_columns.begin(); it != j_columns.end(); it++) {
        std::string col_id = (*it)["id"].get<std::string>();
        std::string col_name = (*it)["name"].get<std::string>();
        std::string col_datatype = (*it)["datatype"].get<std::string>();

        columns_.push_back(ColumnDesc(col_id, col_name, col_datatype));
    }
}

}  // namespace cube
}  // namespace husky
