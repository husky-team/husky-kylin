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

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "core-metadata/metadata/model/model_dimension_desc.hpp"
#include "core-metadata/metadata/model/table_desc.hpp"
#include "core-metadata/metadata/model/table_ref.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

class DataModelDesc {
   public:
    DataModelDesc() {}
    DataModelDesc(const std::string& modelJsonPath, const std::string& tableJsonPath);
    ~DataModelDesc() {}

    inline const std::string& get_name() const { return name_; }
    // std::string getOwner();
    // void setOwner(const std::string& owner);

    std::shared_ptr<TableRef> get_root_fact_table_ref() const { return root_fact_table_ref_; }
    TableRef* find_table(std::string& table) const;  // has side effect on table
    TblColRef* find_column(std::string& table, std::string& column) const;
    TblColRef* find_column(std::string& column) const;

   private:
    /*For now, suppose only one fact table, no lookup tables*/

    // from json
    std::string name_;
    // std::string owner;
    std::string root_fact_table_;  // "fact_table"
    // std::vector<JoinTableDesc *> jointTables; //"lookups"
    std::list<ModelDimensionDesc> dimensions_;  // "dimensions"
    std::vector<std::string> metrics_;          // "metrics"

    // computed attributes
    std::shared_ptr<TableRef> root_fact_table_ref_;
    // std::set<TableRef *> factTableRefs;
    // std::set<TableRef *> loopkupTableRefs;
    std::set<std::shared_ptr<TableRef>> all_table_refs_;
    // std::map<std::string, TableRef* > aliasMap; // alias => TableRef, a table has exactly one alias
    std::map<std::string, TableRef*> table_name_map_;  // name => TableRef, a table maybe referenced by multiple names
    // JoinsTree * joinsTree;

    void add_table_name(const std::string& name, const std::shared_ptr<TableRef>& ref);
    // void addAlias(TableRef * ref);
    // void initRootTable(TableDesc * rootTableDesc);
    // void initDimensionAndMetrics();
    // void init(TableDesc * rootTableDesc);
};

}  // namespace cube
}  // namespace husky
