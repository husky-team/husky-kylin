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

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "core-metadata/metadata/model/column_desc.hpp"

namespace husky {
namespace cube {

using nlohmann::json;

class TableDesc {
   public:
    explicit TableDesc(const json& table_json_path);
    ~TableDesc() {}
    // TabeleDesc * appendColumns(std::vector<ColumnDesc* > computedColumns, bool makeCopy);
    inline std::vector<ColumnDesc>& get_columns() { return columns_; }
    inline const std::vector<ColumnDesc>& get_columns() const { return columns_; }
    inline const std::string& get_name() const { return name_; }

   private:
    std::string name_;
    std::vector<ColumnDesc> columns_;
    // std::string tableType;
};

}  // namespace cube
}  // namespace husky
