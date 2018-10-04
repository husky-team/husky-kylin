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
#include <vector>

#include "core-cube/model/row_key_col_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

class CubeDesc;

class RowKeyDesc {
   public:
    explicit RowKeyDesc(CubeDesc* cube_desc) {}
    ~RowKeyDesc();

    inline const std::vector<RowKeyColDesc*>& get_row_key_columns() const { return row_key_columns_; }
    inline int get_column_bit_index(TblColRef* col) const { return 0; /*TODO(tatiana)*/ }
    inline RowKeyColDesc* get_col_desc(TblColRef* col) const { return nullptr; /*TODO(tatiana)*/ }
    inline uint64_t get_full_mask() const { return full_mask_; /*TODO(tatiana)*/ }
    inline const std::vector<int>& get_columns_need_index() const { return columns_need_index_; /*TODO(tatiana)*/ }

    inline void set_cube_desc(CubeDesc* cube_desc) { /*TODO(tatiana)*/
    }
    inline void set_row_key_columns(const std::vector<RowKeyColDesc*>& row_key_columns) { /*TODO(tatiana)*/
    }

   private:
    void init_columns_need_index() { /*TODO(tatiana)*/
    }
    void build_row_key() { /*TODO(tatiana)*/
    }

    std::vector<RowKeyColDesc*> row_key_columns_;
    uint64_t full_mask_;
    std::map<TblColRef*, RowKeyColDesc*> column_map_;
    std::vector<int> columns_need_index_;
};

}  // namespace cube
}  // namespace husky
