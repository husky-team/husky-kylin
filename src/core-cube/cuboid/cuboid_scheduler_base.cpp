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

#include "core-cube/cuboid/cuboid_scheduler_base.hpp"

#include <vector>

#include "glog/logging.h"

#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

std::vector<std::vector<uint64_t>> CuboidSchedulerBase::get_cuboids_by_layer() {
    if (!cuboids_by_layer_.empty()) {
        return cuboids_by_layer_;
    }

    int total_num = 0;
    std::vector<uint64_t> base_cuboid_vec;
    base_cuboid_vec.push_back(get_base_cuboid_id());
    cuboids_by_layer_.push_back(base_cuboid_vec);
    total_num++;

    std::vector<uint64_t>& last_layer = base_cuboid_vec;
    while (!last_layer.empty()) {
        std::vector<uint64_t> new_layer;
        for (auto& parent : last_layer) {
            const std::vector<uint64_t> spanning_cuboid = get_spanning_cuboid(parent);
            new_layer.insert(new_layer.end(), spanning_cuboid.begin(), spanning_cuboid.end());
        }
        if (new_layer.empty()) {
            break;
        }
        cuboids_by_layer_.push_back(new_layer);
        total_num += new_layer.size();
        last_layer = new_layer;
    }

    auto size = get_all_cuboid_ids().size();
    CHECK_EQ(total_num, size);
    return cuboids_by_layer_;
}

}  // namespace cube
}  // namespace husky
