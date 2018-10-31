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

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "core-cube/model/row_key_col_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

class CubeDesc;

class RowKeyDesc {
   public:
    RowKeyDesc() {
        // init_columns_need_index(); // about columns using "dictionary"
    }
    ~RowKeyDesc() {}

    inline const std::vector<std::shared_ptr<RowKeyColDesc>>& get_row_key_columns() const { return row_key_columns_; }
    inline int get_column_bit_index(const std::shared_ptr<TblColRef>& col) const {
        return get_col_desc(col)->get_bit_index();
    }
    inline std::shared_ptr<RowKeyColDesc> get_col_desc(const std::shared_ptr<TblColRef>& col) const {
        auto col_desc_itr = column_map_.find(col);
        if (col_desc_itr == column_map_.end()) {
            // throw some exception
        }
        return col_desc_itr->second;
    }
    inline uint64_t get_full_mask() const { return full_mask_; }
    // inline const std::vector<int>& get_columns_need_index() const { return columns_need_index_; }

    inline void set_cube_desc(CubeDesc* cube_desc) {
        cube_desc_ = cube_desc;
        build_row_key();
    }
    inline void add_row_key_col(const std::shared_ptr<RowKeyColDesc>& row_key_col) {
        row_key_columns_.push_back(row_key_col);
    }

   private:
    // void init_columns_need_index() {}
    void build_row_key();

    std::vector<std::shared_ptr<RowKeyColDesc>> row_key_columns_;
    CubeDesc* cube_desc_;  // not owned, this is my owner
    uint64_t full_mask_;
    std::map<std::shared_ptr<TblColRef>, std::shared_ptr<RowKeyColDesc>> column_map_;
    // std::vector<int> columns_need_index_;
};

}  // namespace cube
}  // namespace husky
