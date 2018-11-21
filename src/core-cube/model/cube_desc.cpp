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

#include "core-cube/model/cube_desc.hpp"

#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "nlohmann/json.hpp"

#include "core-cube/model/dimension_desc.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/function_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"
#include "core-metadata/metadata/model/parameter_desc.hpp"

namespace husky {
namespace cube {

using nlohmann::json;

CubeDesc::CubeDesc(const json& j) {
    name_ = j["name"].get<std::string>();
    model_name_ = j["model_name"].get<std::string>();
    if (j.find("parent_forward") != j.end()) {
        parent_forward_ = j["parent_forward"].get<int>();
    }

    // Init dimensions
    json j_dimensions = j["dimensions"];
    for (json::iterator it = j_dimensions.begin(); it != j_dimensions.end(); it++) {
        std::string d_name = (*it)["name"].get<std::string>();

        // empty string for derived columns
        std::string d_column = (*it)["column"].is_null() ? "" : (*it)["column"].get<std::string>();
        std::string d_table = (*it)["table"].get<std::string>();

        dimensions_.push_back(std::make_shared<DimensionDesc>(d_name, d_table, d_column));
    }

    // Init measures
    json j_measures = j["measures"];
    for (auto& j_measure : j_measures) {
        std::string m_name = j_measure["name"].get<std::string>();

        json j_function = j_measure["function"];
        std::string f_expression = j_function["expression"].get<std::string>();
        std::string f_returnType = j_function["returntype"].get<std::string>();

        std::shared_ptr<FunctionDesc> function_desc = std::make_shared<FunctionDesc>(
            f_expression, std::make_unique<ParameterDesc>(j_function["parameter"]), f_returnType);

        measures_.push_back(std::make_shared<MeasureDesc>(m_name, function_desc));
    }

    // init row_key_columns
    row_key_ = std::make_shared<RowKeyDesc>();
    json j_rowkey_columns = j["rowkey"]["rowkey_columns"];
    for (json::iterator it = j_rowkey_columns.begin(); it != j_rowkey_columns.end(); it++) {
        std::string column = (*it)["column"].get<std::string>();
        std::string encoding = (*it)["encoding"].get<std::string>();

        row_key_->add_row_key_col(std::make_shared<RowKeyColDesc>(column, encoding));
    }

    // init aggregation groups
    json j_aggregation_groups = j["aggregation_groups"];
    for (auto& j_agg : j_aggregation_groups) {
        aggregation_groups_.push_back(std::make_unique<AggregationGroup>(j_agg));
    }
}

const std::vector<AggregationGroup*> CubeDesc::get_aggregation_group_ptrs() const {
    std::vector<AggregationGroup*> ret;
    ret.reserve(aggregation_groups_.size());
    for (auto& agg : aggregation_groups_) {
        ret.push_back(agg.get());
    }
    return ret;
}

void CubeDesc::init(const json& model_json, const std::map<std::string, std::shared_ptr<json>>& table_jsons) {
    model_ = std::make_shared<DataModelDesc>();
    model_->init(model_json, table_jsons);

    for (auto dimension : dimensions_) {
        dimension->init(this);
    }
    init_dimension_columns();
    init_measure_columns();

    row_key_->set_cube_desc(this);
    // row_key_->set_row_key_columns(row_key_columns_);

    for (auto& agg : aggregation_groups_) {
        agg->init(this, row_key_.get());
    }
}

void CubeDesc::init_dimension_columns() {
    for (auto& dim : dimensions_) {
        if (dim->is_derived()) {
            auto& foreign_keys = dim->get_table_ref()->get_foreign_keys();
            for (auto foreign_key : foreign_keys) {
                init_dimension_col_ref(model_->find_column(foreign_key));
            }
        } else {
            init_dimension_col_ref(dim->get_column_ref());
        }
    }
}

void CubeDesc::init_dimension_col_ref(const std::shared_ptr<TblColRef>& col_ref) {
    CHECK_EQ(true, col_ref != nullptr);
    uint32_t idx = dimension_columns_map_.size();
    if (dimension_columns_map_.insert({col_ref, idx}).second) {
        dimension_columns_.push_back(col_ref);
    }
}

void CubeDesc::init_measure_columns() {
    for (auto& measure : measures_) {
        auto name = measure->get_name();
        boost::to_upper(name);
        measure->set_name(name);
        measure->get_function()->init(*model_);
    }
}

std::vector<std::shared_ptr<TblColRef>> CubeDesc::get_flat_table_columns() const {
    // dimensions
    std::vector<std::shared_ptr<TblColRef>> ret;
    ret.insert(ret.end(), dimension_columns_.begin(), dimension_columns_.end());

    // measures
    std::set<std::shared_ptr<TblColRef>> measures;
    for (auto& measure : measures_) {
        const auto& col_refs = measure->get_function()->get_parameter()->get_col_refs();
        for (auto& col_ref : col_refs) {
            if (dimension_columns_map_.count(col_ref) == 0 && measures.count(col_ref) == 0) {
                ret.push_back(col_ref);
                measures.insert(col_ref);
            }
        }
    }

    return ret;
}

}  // namespace cube
}  // namespace husky
