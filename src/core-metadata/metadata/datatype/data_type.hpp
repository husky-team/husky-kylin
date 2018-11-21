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

namespace husky {
namespace cube {

class DataType {
    // support integer, number(float family) and string
   public:
    DataType(const std::string& name, int precision, int scale);
    explicit DataType(const std::string& dataType);
    ~DataType() {}

    // DATETIME_FAMILY
    static const std::set<std::string> INTEGER_FAMILY;
    static const std::set<std::string> NUMBER_FAMILY;
    static const std::set<std::string> STRING_FAMILY;
    static std::shared_ptr<DataType> get_type(const std::string& type);

    inline const std::string& get_name() const { return name_; }
    inline int get_precision() const { return precision_; }
    inline int get_scale() const { return scale_; }

    inline bool is_string_family() const { return STRING_FAMILY.find(name_) != STRING_FAMILY.end(); }
    inline bool is_integer_family() const { return INTEGER_FAMILY.find(name_) != INTEGER_FAMILY.end(); }
    inline bool is_number_family() const { return NUMBER_FAMILY.find(name_) != NUMBER_FAMILY.end(); }
    inline bool is_tiny_int() const { return name_.compare("tinyint") == 0; }
    inline bool is_small_int() const { return name_.compare("smallint") == 0; }
    inline bool is_int() const { return name_.compare("int") == 0 || name_.compare("integer") == 0; }
    inline bool is_big_int() const { return name_.compare("bigint") == 0; }
    inline bool is_float() const { return name_.compare("float") == 0; }
    inline bool is_double() const { return name_.compare("double") == 0; }
    inline bool is_decimal() const { return name_.compare("decimal") == 0; }
    inline bool is_boolean() const { return name_.compare("boolean") == 0; }

   private:
    std::string name_;
    int precision_;
    int scale_;
};

}  // namespace cube
}  // namespace husky
