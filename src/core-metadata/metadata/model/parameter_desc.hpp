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

#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

class ParameterDesc {
   public:
    ParameterDesc(const std::string& type, const std::string& value);
    ~ParameterDesc() {}

    inline const std::string& get_type() const { return type_; }
    inline void set_type(const std::string& type) { type_ = type; }
    inline const std::string& get_value() const { return value_; }
    inline void set_value(const std::string& value) { value_ = value; }
    inline const std::shared_ptr<TblColRef>& get_col_ref() const { return col_ref_; }
    inline void set_tbl_col_ref(const std::shared_ptr<TblColRef>& col_ref) { col_ref_ = col_ref; }
    bool is_column_type() const;

   private:
    std::string type_;
    std::string value_;

    // computed attributes
    std::shared_ptr<TblColRef> col_ref_;
};

}  // namespace cube
}  // namespace husky
