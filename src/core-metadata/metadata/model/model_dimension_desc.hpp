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

#include "boost/algorithm/string.hpp"

namespace husky {
namespace cube {

class DataModelDesc;

class ModelDimensionDesc {
   public:
    ModelDimensionDesc(const std::string& table, const std::vector<std::string>& columns);
    ~ModelDimensionDesc() {}

    inline const std::string& get_table() { return table_; }
    inline void set_table(const std::string& table) { table_ = table; }
    inline const std::vector<std::string>& get_columns() { return columns_; }
    inline void set_columns(const std::vector<std::string>& columns) { columns_ = columns; }

    // TODO(tatiana):
    // void init(DataModelDesc * model);

   private:
    std::string table_;
    std::vector<std::string> columns_;
};

}  // namespace cube
}  // namespace husky
