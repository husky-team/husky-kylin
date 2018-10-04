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

#include "core-metadata/measure/measure_type.hpp"
#include "core-metadata/metadata/datatype/data_type.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/parameter_desc.hpp"

#include "boost/algorithm/string.hpp"

namespace husky {
namespace cube {

const char FunctionDesc::FUNC_SUM[] = "SUM";
const char FunctionDesc::FUNC_MIN[] = "MIN";
const char FunctionDesc::FUNC_MAX[] = "MAX";
const char FunctionDesc::FUNC_COUNT[] = "COUNT";

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
}

void FunctionDesc::init(DataModelDesc* model) {
    boost::to_upper(expression_);
    return_data_type_ = DataType::get_type(return_type_);
    // TODO(tatiana): multiple parameters
    if (parameter_->is_column_type()) {
        std::string value = parameter_->get_value();
        TblColRef* colRef = model->find_column(value);
        // p.setValue(colRef.get_identity());
        parameter_->set_tbl_col_ref(colRef);
    }
}

void FunctionDesc::set_return_type(const std::string& return_type) {
    this->return_type_ = return_type;
    this->return_data_type_ = DataType::get_type(return_type_);
}

}  // namespace cube
}  // namespace husky
