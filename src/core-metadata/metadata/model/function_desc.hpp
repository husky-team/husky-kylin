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
#include <set>
#include <string>

#include "core-metadata/measure/measure_type.hpp"
#include "core-metadata/metadata/datatype/data_type.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/parameter_desc.hpp"

namespace husky {
namespace cube {

class FunctionDesc {
   public:
    FunctionDesc(const std::string& expression, std::unique_ptr<ParameterDesc>&& parameter,
                 const std::string& return_type);
    ~FunctionDesc() {}

    static const char FUNC_SUM[];
    static const char FUNC_MIN[];
    static const char FUNC_MAX[];
    static const char FUNC_COUNT[];

    static const std::set<std::string> BUILD_IN_AGGREGATIONS;
    static const char PARAMETER_TYPE_CONSTANT[];
    static const char PARAMETER_TYPE_COLUMN[];

    void init(DataModelDesc* model);

    inline bool is_min() const { return expression_.compare(FUNC_MIN) == 0; }
    inline bool is_max() const { return expression_.compare(FUNC_MAX) == 0; }
    inline bool is_sum() const { return expression_.compare(FUNC_SUM) == 0; }
    inline bool is_count() const { return expression_.compare(FUNC_COUNT) == 0; }

    inline const std::string& get_expression() const { return expression_; }
    inline void set_expression(const std::string& expression) { this->expression_ = expression; }
    inline ParameterDesc* get_parameter() const { return parameter_.get(); }
    inline void set_parameter(std::unique_ptr<ParameterDesc>&& parameter) { this->parameter_ = std::move(parameter); }
    inline const std::string& get_return_type() const { return return_type_; }
    inline DataType* get_return_data_type() { return return_data_type_.get(); }
    void set_return_type(const std::string& return_type);

   private:
    // from json
    std::string expression_;
    std::unique_ptr<ParameterDesc> parameter_;  // just one parameter
    std::string return_type_;

    // computed attributes
    std::shared_ptr<DataType> return_data_type_;
    MeasureType measure_type_;
};

}  // namespace cube
}  // namespace husky
