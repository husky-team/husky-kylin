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

#include "core-metadata/metadata/datatype/data_type.hpp"

#include <memory>
#include <set>
#include <string>

namespace husky {
namespace cube {

const std::set<std::string> DataType::INTEGER_FAMILY{"tinyint", "smallint", "integer", "bigint", "int4", "long8"};
const std::set<std::string> DataType::NUMBER_FAMILY{"tinyint", "smallint", "integer", "bigint", "int4",   "long8",
                                                    "float",   "double",   "decimal", "real",   "numeric"};
const std::set<std::string> DataType::STRING_FAMILY{"varchar", "char"};

DataType::DataType(const std::string& name, int precision, int scale)
    : name_(name), precision_(precision), scale_(scale) {}

DataType::DataType(const std::string& data_type) {
    name_ = data_type;
    precision_ = -1;
    scale_ = -1;

    auto pos = data_type.find("decimal");
    if (pos != name_.npos) {
        name_ = "decimal";
        pos = data_type.find("(");
        if (pos != name_.npos) {
            auto comma = data_type.find(",");
            precision_ = std::stoi(data_type.substr(pos + 1, comma - pos - 1));
            pos = data_type.find(")");
            scale_ = std::stoi(data_type.substr(comma + 1, pos - comma - 1));
        }
        return;
    }

    // kylin's default precision
    if (name_.compare("char") == 0) {
        precision_ = 255;
    } else if (name_.compare("varchar") == 0) {
        precision_ = 256;
    } else if (name_.compare("decimal") == 0 || name_.compare("numeric") == 0) {
        precision_ = 19;
        scale_ = 4;
    }
}

std::shared_ptr<DataType> DataType::get_type(const std::string& type) {
    if (type.empty())
        return nullptr;
    return std::make_shared<DataType>(type);
}

}  // namespace cube
}  // namespace husky
