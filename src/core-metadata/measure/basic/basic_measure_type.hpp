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

#include "glog/logging.h"

#include "core-metadata/measure/basic/sum_aggregators.hpp"
#include "core-metadata/measure/measure_aggregators.hpp"
#include "core-metadata/measure/measure_type.hpp"
#include "core-metadata/metadata/datatype/data_type.hpp"
#include "core-metadata/metadata/model/constants.hpp"

namespace husky {
namespace cube {

class BasicMeasureType : public MeasureType {
   public:
    BasicMeasureType(const std::string& func_name, const std::shared_ptr<DataType>& return_type)
        : func_name_(func_name), data_type_(return_type) {}

    virtual ~BasicMeasureType() {}

    std::shared_ptr<MeasureAggregator> new_aggregator() const override {
        if (func_name_ == FunctionExpr::FUNC_COUNT || func_name_ == FunctionExpr::FUNC_SUM) {
            if (data_type_->is_decimal()) {
                return std::make_shared<BigDecimalSumAggregator>();
            } else if (data_type_->is_integer_family()) {
                return std::make_shared<LongSumAggregator>();
            } else if (data_type_->is_number_family()) {
                return std::make_shared<DoubleSumAggregator>();
            }
            LOG(WARNING) << "func_name_ " << func_name_ << " data_type_ " << data_type_->get_name()
                         << " not implemented";
        } else if (func_name_ == FunctionExpr::FUNC_MAX) {
            // TODO(tatiana)
            LOG(WARNING) << "MAX aggregator not implemented";
        } else if (func_name_ == FunctionExpr::FUNC_MIN) {
            LOG(WARNING) << "MIN aggregator not implemented";
        } else {
            LOG(ERROR) << "No aggregator for func " << func_name_ << " and return type " << data_type_->get_name();
        }
        return std::make_shared<MeasureAggregator>();
    }

    void validate() const override {
        if (func_name_ == FunctionExpr::FUNC_SUM) {
            CHECK_EQ(true, data_type_->is_number_family()) << "Return type for " << func_name_
                                                           << " must be of number family";
        } else if (func_name_ == FunctionExpr::FUNC_COUNT) {
            CHECK_EQ(true, data_type_->is_integer_family()) << "Return type for " << func_name_
                                                            << " must be of integer family";
        } else if (func_name_ == FunctionExpr::FUNC_MAX || func_name_ == FunctionExpr::FUNC_MIN) {
            CHECK_EQ(true, data_type_->is_number_family()) << "Return type for " << func_name_
                                                           << " must be of number family";
        } else {
            LOG(WARNING) << "Unknown function " << func_name_;
        }
    }

   private:
    std::string func_name_;
    std::shared_ptr<DataType> data_type_;
};

}  // namespace cube
}  // namespace husky
