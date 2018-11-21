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

#include "core-metadata/metadata/model/function_desc.hpp"

#include <set>
#include <string>

#include "boost/algorithm/string.hpp"

#include "core-metadata/measure/measure_type.hpp"
#include "core-metadata/measure/measure_type_factory.hpp"
#include "core-metadata/metadata/datatype/data_type.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/parameter_desc.hpp"

namespace husky {
namespace cube {

const std::set<std::string> FunctionDesc::BUILD_IN_AGGREGATIONS{"SUM", "MIN", "MAX", "COUNT"};

const char FunctionDesc::PARAMETER_TYPE_CONSTANT[] = "constant";
const char FunctionDesc::PARAMETER_TYPE_COLUMN[] = "column";

FunctionDesc::FunctionDesc(const std::string& expression, std::unique_ptr<ParameterDesc>&& parameter,
                           const std::string& return_type)
    : expression_(expression),
      parameter_(std::move(parameter)),
      return_type_(return_type),
      return_data_type_(DataType::get_type(return_type)) {
    boost::to_upper(expression_);
    measure_type_ = MeasureTypeFactory::create(expression_, return_data_type_);
}

void FunctionDesc::init(const DataModelDesc& model) {
    boost::to_upper(expression_);
    return_data_type_ = DataType::get_type(return_type_);
    auto parameter = parameter_.get();
    while (parameter != nullptr) {
        if (parameter->is_column_type()) {
            std::string value = parameter->get_value();
            std::shared_ptr<TblColRef> col_ref = model.find_column(value);
            parameter->set_value(col_ref->get_identity());
            parameter->set_tbl_col_ref(col_ref);
        }
        parameter = parameter->next_parameter().get();
    }
}

void FunctionDesc::set_return_type(const std::string& return_type) {
    this->return_type_ = return_type;
    this->return_data_type_ = DataType::get_type(return_type_);
}

}  // namespace cube
}  // namespace husky
