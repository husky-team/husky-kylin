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

#include "core-metadata/metadata/model/parameter_desc.hpp"

#include <string>

#include "core-metadata/metadata/model/function_desc.hpp"

namespace husky {
namespace cube {

ParameterDesc::ParameterDesc(const std::string& type, const std::string& value) : type_(type), value_(value) {}

bool ParameterDesc::is_column_type() const { return type_.compare(FunctionDesc::PARAMETER_TYPE_COLUMN) == 0; }

}  // namespace cube
}  // namespace husky
