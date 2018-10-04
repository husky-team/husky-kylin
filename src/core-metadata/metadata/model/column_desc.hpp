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

namespace husky {
namespace cube {

class ColumnDesc {
   public:
    ColumnDesc(const std::string& id, const std::string& name, const std::string& datatype);
    ~ColumnDesc() {}

    inline const std::string& get_id() const { return id_; }
    inline const std::string& get_name() const { return name_; }
    inline const std::string& get_data_type() const { return data_type_; }

   private:
    std::string id_;
    std::string name_;
    std::string data_type_;
};

}  // namespace cube
}  // namespace husky
