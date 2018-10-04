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

#include "core-metadata/metadata/model/table_ref.hpp"

#include <string>
#include <vector>

#include "glog/logging.h"

#include "core-metadata/metadata/model/column_desc.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/table_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

TableRef::TableRef(DataModelDesc* model, const std::string& alias, TableDesc&& table)
    : model_(model), table_(table), alias_(alias) {
    model_name_ = model->get_name();
    std::vector<ColumnDesc>& column_vector = table_.get_columns();
    for (std::vector<ColumnDesc>::iterator it = column_vector.begin(); it != column_vector.end(); ++it) {
        columns_.insert(std::make_pair(it->get_name(), TblColRef(this, &(*it))));
    }
}

TblColRef* TableRef::get_column(const std::string& name) {
    auto pos = columns_.find(name);
    if (pos == columns_.end()) {
        return nullptr;
    }
    return &(pos->second);
}

}  // namespace cube
}  // namespace husky
