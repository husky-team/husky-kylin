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

#include "core-metadata/metadata/model/data_model_desc.hpp"

#include <fstream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "nlohmann/json.hpp"

#include "core-metadata/metadata/model/model_dimension_desc.hpp"
#include "core-metadata/metadata/model/table_desc.hpp"
#include "core-metadata/metadata/model/table_ref.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

using json = nlohmann::json;

void DataModelDesc::add_table_name(const std::string& name, const std::shared_ptr<TableRef>& ref) {
    if (table_name_map_.count(name)) {
        table_name_map_.insert(std::make_pair(name, std::shared_ptr<TableRef>()));
    } else {
        table_name_map_.insert(std::make_pair(name, ref));
        all_table_refs_.insert(ref);
    }
}

// void DataModelDesc::addAlias(TableRef * ref) {
//   std::string alias = ref->getAlias();
//   if(aliasMap.find(alias) != alias.end())
//     throw std::logic_error("Alias '" + alias + "' ref to multiple tables")
//   aliasMap.insert(std::make_pair(alias, ref));

//   TableDesc * table = ref->getTableDesc();
//   add_table_name(table->get_name(), ref);
//   add_table_name(table->get_identity(), ref);
// }

std::shared_ptr<TableRef> DataModelDesc::find_table(std::string& table) const {
    boost::to_upper(table);
    std::shared_ptr<TableRef> result = table_name_map_.find(table)->second;
    // error detection: result == NULL
    return result;
}

std::shared_ptr<TblColRef> DataModelDesc::find_column(std::string& table, std::string& column) const {
    std::shared_ptr<TableRef> tableRef = find_table(table);
    boost::to_upper(column);
    return tableRef->get_column(column);
}

std::shared_ptr<TblColRef> DataModelDesc::find_column(std::string& column) const {
    // TblColRef* result = nullptr;
    std::shared_ptr<TblColRef> result;
    result.reset();
    // std::string input = column;

    boost::to_upper(column);
    int cut = column.rfind('.');
    if (cut == std::string::npos) {
        // table not specified, try each table
        for (std::set<std::shared_ptr<TableRef>>::iterator it = all_table_refs_.begin(); it != all_table_refs_.end();
             ++it) {
            result = (*it)->get_column(column);
            if (result != NULL)
                return result;
        }
    } else {
        // table specified
        std::string tableName = column.substr(0, cut);
        std::string columnName = column.substr(cut + 1);
        return find_column(tableName, columnName);
    }

    return result;
}

void DataModelDesc::init(const std::string& model_json_path, const std::string& table_json_path) {
    std::ifstream ifs(model_json_path);
    json j = json::parse(ifs);

    name_ = j["name"].get<std::string>();
    // owner = j["owner"].get<std::string>();
    root_fact_table_ = j["fact_table"].get<std::string>();

    // init root_fact_table_ref_
    TableDesc root_fact_table_desc(table_json_path);
    std::string root_fact_table_name = root_fact_table_desc.get_name();
    root_fact_table_ref_ = std::make_shared<TableRef>(this, root_fact_table_name, std::move(root_fact_table_desc));
    add_table_name(root_fact_table_name, root_fact_table_ref_);

    // init dimensions
    json j_dimensions = j["dimensions"];
    for (json::iterator it = j_dimensions.begin(); it != j_dimensions.end(); it++) {
        std::string table = (*it)["table"].get<std::string>();
        // should add_table_name(table) ? need more table.json to test!

        json j_columns = (*it)["columns"];
        std::vector<std::string> columns;
        for (json::iterator it = j_columns.begin(); it != j_columns.end(); it++) {
            columns.push_back((*it).get<std::string>());
        }

        dimensions_.push_back(ModelDimensionDesc(table, columns));
    }

    // init metrics
    json j_metrics = j["metrics"];
    for (json::iterator it = j_metrics.begin(); it != j_metrics.end(); it++) {
        std::string metric_name = (*it).get<std::string>();
        metrics_.push_back(find_column(metric_name)->get_identity());
    }
}

// void DataModelDesc::initRootTable(TableDesc * rootTableDesc) {
//   root_fact_table_ref_ = new TableRef(this, rootDesc.get_name(), rootDesc);
//   add_table_name(rootTableDesc->get_name(), root_fact_table_ref_);
// }

// void DataModelDesc::initDimensionAndMetrics();

// void DataModelDesc::initTableAlias(std::map<std::string, TableDesc * >& tables) {
//   if(root_fact_table_.empty())
//     throw std::logic_error("root fact table should not be empty");

//   boost::to_upper(root_fact_table_);
//   if (tables.find(root_fact_table_) == tables.end())
//     throw std::logic_error("Root fact table does not exist:" + root_fact_table_);

//   TableDesc * rootDesc = tables.find(root_fact_table_);
//   root_fact_table_ref_ = new TableRef(this, rootDesc.get_name(), rootDesc)
//   addAlias(root_fact_table_ref_);
// }

}  // namespace cube
}  // namespace husky
