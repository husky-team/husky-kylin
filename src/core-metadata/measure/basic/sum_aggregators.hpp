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

#include <string>

#include "glog/logging.h"

#include "core-metadata/measure/measure_aggregators.hpp"
#include "core-metadata/metadata/datatype/data_type.hpp"
#include "core-metadata/metadata/model/function_desc.hpp"

namespace husky {
namespace cube {

class BigDecimalSumAggregator : public MeasureAggregator {
   public:
    /* TODO(tatiana) now treated as double, try to have java BigDecimal implementation for better precision;*/
    void aggregate(std::string& val_str, const std::string& inc_str) const {
        val_str = std::to_string(std::stod(val_str) + std::stod(inc_str));
    }
};

class LongSumAggregator : public MeasureAggregator {
   public:
    void aggregate(std::string& val_str, const std::string& inc_str) const {
        val_str = std::to_string(std::stoll(val_str) + std::stoll(inc_str));
    }
};

class DoubleSumAggregator : public MeasureAggregator {
   public:
    void aggregate(std::string& val_str, const std::string& inc_str) const {
        val_str = std::to_string(std::stod(val_str) + std::stod(inc_str));
    }
};

}  // namespace cube
}  // namespace husky
