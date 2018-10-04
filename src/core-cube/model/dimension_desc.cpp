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

#include "core-cube/model/dimension_desc.hpp"

#include <string>

#include "core-metadata/metadata/model/data_model_desc.hpp"

namespace husky {
namespace cube {

DimensionDesc::DimensionDesc(const std::string& name, const std::string& table, const std::string& column)
    : name_(name), table_(table), column_(column) {
    // Init table
    // TODO(tatiana):
    // DataModelDesc* model = cubeDesc.getModel();
    // table_ref_ = model.findTable(table);

    // Init column
    // column = model.findColumn(table, column);
}

}  // namespace cube
}  // namespace husky
