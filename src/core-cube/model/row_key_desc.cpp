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

#include "core-cube/model/row_key_desc.hpp"
#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

// RowKeyDesc::RowKeyDesc() {
//     // init_columns_need_index(); // about columns using "dictionary"
//     row_key_columns_ = cube_desc->get_row_key_columns();
//     build_row_key();
// }

void RowKeyDesc::build_row_key() {
    for (int i = 0; i < row_key_columns_.size(); i++) {
        std::shared_ptr<RowKeyColDesc> row_key_col_desc = row_key_columns_[i];
        row_key_col_desc->init(row_key_columns_.size() - i - 1, cube_desc_);
        column_map_.insert(std::make_pair(row_key_col_desc->get_col_ref(), row_key_col_desc));
    }

    full_mask_ = 0L;
    for (int i = 0; i < row_key_columns_.size(); i++) {
        int index = row_key_columns_[i]->get_bit_index();
        full_mask_ |= 1L << index;
    }
}

}  // namespace cube
}  // namespace husky
