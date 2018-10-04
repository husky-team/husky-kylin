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

#include "core-metadata/metadata/model/model_dimension_desc.hpp"

#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"

// #include "core-metadata/metadata/model/data_model_desc.hpp"

namespace husky {
namespace cube {

ModelDimensionDesc::ModelDimensionDesc(const std::string& table, const std::vector<std::string>& columns)
    : table_(table), columns_(columns) {
    boost::to_upper(table_);
}

// void ModelDimensionDesc::init(DataModelDesc * model) {
//  boost::toupper(table);
// }

}  // namespace cube
}  // namespace husky
