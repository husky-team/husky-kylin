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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core-metadata/metadata/model/table_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"
// #include "DataModelDesc.hpp"

namespace husky {
namespace cube {

class DataModelDesc;

class TableRef {
   public:
    TableRef(DataModelDesc* model, const std::string& alias, TableDesc&& table);
    ~TableRef() {}

    std::shared_ptr<TblColRef> get_column(const std::string& name) const;
    inline const std::string& get_alias() const { return alias_; }
    const TableDesc& get_table_desc() { return table_; }
    inline const std::string& get_table_name() const { return table_.get_name(); }

    inline void set_foreign_keys(std::vector<std::string>&& foreign_keys) { foreign_keys_ = std::move(foreign_keys); }
    inline const std::vector<std::string>& get_foreign_keys() const { return foreign_keys_; }

   private:
    std::string model_name_;
    std::string alias_;
    DataModelDesc* model_;  // not owned, this is my owner
    TableDesc table_;
    std::map<std::string, std::shared_ptr<TblColRef>> columns_;

    std::vector<std::string> foreign_keys_;
};

}  // namespace cube
}  // namespace husky
