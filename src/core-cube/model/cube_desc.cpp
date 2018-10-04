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
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

#include "core-cube/model/dimension_desc.hpp"
#include "core-cube/model/row_key_desc.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/function_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"
#include "core-metadata/metadata/model/parameter_desc.hpp"

namespace husky {
namespace cube {

CubeDesc::CubeDesc(const std::string& cubeDescJsonPath) {
    std::ifstream ifs(cubeDescJsonPath);
    json j = json::parse(ifs);

    name_ = j["name"].get<std::string>();
    model_name_ = j["model_name"].get<std::string>();
    if (!j["parent_forward"].is_null())
        parent_forward_ = j["parent_forward"].get<int>();

    // Init model
    model_ = std::make_shared<DataModelDesc>();

    // Init dimensions
    json j_dimensions = j["dimensions"];
    for (json::iterator it = j_dimensions.begin(); it != j_dimensions.end(); it++) {
        std::string d_name = (*it)["name"].get<std::string>();
        std::string d_column = (*it)["column"].get<std::string>();
        std::string d_table = (*it)["table"].get<std::string>();

        dimensions_.push_back(DimensionDesc(d_name, d_table, d_column));
    }

    // Init measures
    json j_measures = j["measures"];
    for (json::iterator it = j_measures.begin(); it != j_measures.end(); it++) {
        std::string m_name = (*it)["name"].get<std::string>();

        json j_function = (*it)["function"];
        std::string f_expression = j_function["expression"].get<std::string>();
        std::string f_returnType = j_function["returnType"].get<std::string>();

        json j_parameter = j_function["parameter"];
        std::string p_type = j_parameter["type"].get<std::string>();
        std::string p_value = j_parameter["value"].get<std::string>();

        std::shared_ptr<FunctionDesc> function_desc = std::make_shared<FunctionDesc>(
            f_expression, std::make_unique<ParameterDesc>(p_type, p_value), f_returnType);

        measures_.push_back(MeasureDesc(m_name, function_desc));
    }
}

// TODO(alice): seems to be redundant with the contructor, let's discuss which to be moved/put here
// void CubeDesc::init() {
//     // init model
//     DataModelDesc* init_model = new DataModelDesc();
//     setModel(init_model);
//
//     // init dimensions
//     for (std::list<DimensionDesc*>::iterator it = dimensions.begin(); it != dimensions.end(); ++it) {
//         *it->init(this);
//     }
//
//     initDimensionColumns();
//     initMeasureColumns();
//
//     // init rowKey
//     rowKey.init(this);
//
//     // init aggregation columns; implement later
//     //   for(std::list<AggregationGroup* >::iterator it = aggregationGroups.begin(); it !=
//     aggregationGroups.end();
//     ++it)
//     // {
//     //     agg.init(this, rowKey);
//     //   }
// }

}  // namespace cube
}  // namespace husky
