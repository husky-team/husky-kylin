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

#include <list>
#include <memory>
#include <vector>

#include "core-cube/cuboid/cuboid_scheduler.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

Cuboid::Cuboid(const std::shared_ptr<CubeDesc>& cube_desc, uint64_t original_id, uint64_t valid_id)
    : cube_desc_(cube_desc), input_id_(original_id), id_(valid_id) {
    id_bytes_ = utils::long_to_bytes(id_);
    dimension_columns_ = translated_id_to_columns(id_);
}

std::list<TblColRef*> Cuboid::translated_id_to_columns(uint64_t cuboid_id) {
    // TODO(tatiana)
    return std::list<TblColRef*>();
}

}  // namespace cube
}  // namespace husky
