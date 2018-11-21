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

#include <memory>
#include <string>
#include <vector>

#include "core-metadata/metadata/model/table_ref.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

class CubeDesc;

class DimensionDesc {
   public:
    DimensionDesc(const std::string& name, const std::string& table, const std::string& column)
        : name_(name), table_(table), column_(column) {}
    ~DimensionDesc() {}
    void init(CubeDesc* cube_desc);

    inline void set_table(const std::string& table) { table_ = table; }
    inline void set_name(const std::string& name) { name_ = name; }
    inline void set_column(const std::string& column) { column_ = column; }
    inline void set_column_ref(const std::shared_ptr<TblColRef>& column_ref) { column_ref_ = column_ref; }
    inline void set_table_ref(const std::shared_ptr<TableRef>& table_ref) { table_ref_ = table_ref; }

    inline const std::string& get_name() const { return name_; }
    inline const std::string& get_table() const { return table_; }
    inline const std::string& get_column() const { return column_; }
    inline std::shared_ptr<TblColRef> get_column_ref() const { return column_ref_; }
    inline std::shared_ptr<TableRef> get_table_ref() const { return table_ref_; }

    inline bool is_derived() { return column_ == ""; }

   private:
    // from json
    std::string name_;
    std::string table_;
    std::string column_;

    // std::vector<std::string> derived;
    CubeDesc* cube_desc_;  // not owned, this is my owner

    // computed attributes
    std::shared_ptr<TableRef> table_ref_;
    std::shared_ptr<TblColRef> column_ref_;  // suppose no derived columns

    // std::vector<TblColRef* > columnRefs;
};

}  // namespace cube
}  // namespace husky
