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

#include "core-cube/cuboid/cuboid.hpp"

#include <memory>
#include <set>
#include <vector>

#include "core-cube/cuboid/cuboid_scheduler_base.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-cube/model/row_key_col_desc.hpp"
#include "core-cube/model/row_key_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

#include "utils/utils.hpp"

namespace husky {
namespace cube {

Cuboid::Cuboid(const std::shared_ptr<CubeDesc>& cube_desc, uint64_t original_id, uint64_t valid_id)
    : cube_desc_(cube_desc), input_id_(original_id), id_(valid_id) {
    id_bytes_ = utils::long_to_bytes(id_);
    dimension_columns_ = translated_id_to_columns(id_);
}

Cuboid Cuboid::find_cuboid(const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler,
                           const std::set<std::shared_ptr<TblColRef>>& dimensions) {
    uint64_t cuboid_id = to_cuboid_id(cuboid_scheduler->get_cube_desc(), dimensions);
    return Cuboid::find_by_long_id(cuboid_scheduler, cuboid_id);
}

Cuboid Cuboid::find_cuboid(CuboidSchedulerBase* cuboid_scheduler,
                           const std::set<std::shared_ptr<TblColRef>>& dimensions) {
    uint64_t cuboid_id = to_cuboid_id(cuboid_scheduler->get_cube_desc(), dimensions);
    return Cuboid::find_by_long_id(cuboid_scheduler, cuboid_id);
}

Cuboid Cuboid::find_by_bytes_id(const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler,
                                const std::vector<unsigned char>& cuboid_id) {
    return find_by_long_id(cuboid_scheduler, utils::bytes_to_long(cuboid_id));
}

Cuboid Cuboid::find_by_long_id(const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler, uint64_t cuboid_id) {
    uint64_t valid_cuboid_id = cuboid_scheduler->find_best_match_cuboid(cuboid_id);
    return Cuboid(cuboid_scheduler->get_cube_desc(), cuboid_id, valid_cuboid_id);
}

Cuboid Cuboid::find_by_long_id(CuboidSchedulerBase* cuboid_scheduler, uint64_t cuboid_id) {
    uint64_t valid_cuboid_id = cuboid_scheduler->find_best_match_cuboid(cuboid_id);
    return Cuboid(cuboid_scheduler->get_cube_desc(), cuboid_id, valid_cuboid_id);
}

uint64_t Cuboid::get_base_cuboid_id(const std::shared_ptr<CubeDesc>& cube) {
    return cube->get_row_key()->get_full_mask();
}

uint64_t Cuboid::get_base_cuboid_id(CubeDesc* cube) { return cube->get_row_key()->get_full_mask(); }

Cuboid Cuboid::get_base_cuboid(const std::shared_ptr<CubeDesc>& cube) {
    return find_by_long_id(cube->get_initial_cuboid_scheduler(), get_base_cuboid_id(cube));
}

std::vector<std::shared_ptr<TblColRef>> Cuboid::translated_id_to_columns(uint64_t cuboid_id) {
    std::vector<std::shared_ptr<TblColRef>> dimensions;
    std::vector<std::shared_ptr<RowKeyColDesc>> all_columns = cube_desc_->get_row_key()->get_row_key_columns();
    for (int i = 0; i < all_columns.size(); i++) {
        uint64_t bitmask = 1L << all_columns[i]->get_bit_index();
        if ((cuboid_id & bitmask) != 0) {
            std::shared_ptr<TblColRef> col_ref = all_columns[i]->get_col_ref();
            dimensions.push_back(col_ref);
        }
    }
    return dimensions;
}

uint64_t Cuboid::to_cuboid_id(const std::shared_ptr<CubeDesc>& cube_desc,
                              const std::set<std::shared_ptr<TblColRef>>& dimensions) {
    uint64_t cuboid_id = 0;
    for (auto const& column : dimensions) {
        int index = cube_desc->get_row_key()->get_column_bit_index(column);
        cuboid_id |= 1L << index;
    }
    return cuboid_id;
}

}  // namespace cube
}  // namespace husky
