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

#include "core-metadata/measure/basic/basic_measure_type.hpp"
#include "core-metadata/measure/measure_type.hpp"
#include "core-metadata/metadata/datatype/data_type.hpp"

namespace husky {
namespace cube {

class MeasureTypeFactory {
   public:
    static std::unique_ptr<MeasureType> create(const std::string& func_name,
                                               const std::shared_ptr<DataType>& data_type) {
        // TODO(tatiana): support only basic measure now
        return std::make_unique<BasicMeasureType>(func_name, data_type);
    }
};

}  // namespace cube
}  // namespace husky
