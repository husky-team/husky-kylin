
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

#include "core-metadata/metadata/model/column_desc.hpp"

#include <string>

namespace husky {
namespace cube {

ColumnDesc::ColumnDesc(const std::string& id, const std::string& name, const std::string& datatype)
    : id_(id), name_(name), data_type_(datatype) {}

}  // namespace cube
}  // namespace husky
