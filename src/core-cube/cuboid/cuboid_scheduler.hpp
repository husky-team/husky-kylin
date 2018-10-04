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

#include <math.h>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>

#include "core-cube/model/aggregation_group.hpp"
#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

class CuboidScheduler {
   public:
    explicit CuboidScheduler(CubeDesc* cube_desc);
    ~CuboidScheduler() {}

    inline int get_cuboid_count() const { return all_cuboid_ids_.size(); }
    inline std::set<uint64_t>& get_all_cuboid_ids() { return all_cuboid_ids_; }
    inline const std::set<uint64_t>& get_all_cuboid_ids() const { return all_cuboid_ids_; }
    std::list<uint64_t> get_spanning_cuboid(uint64_t cuboid);

    inline bool is_valid(uint64_t requestCuboid) const {
        return all_cuboid_ids_.find(requestCuboid) != all_cuboid_ids_.end();
    }

    /** Returns a valid cuboid that best matches the request cuboid. */
    uint64_t find_best_match_cuboid(uint64_t cuboid) { return find_best_match_cuboid1(cuboid); }
    uint64_t find_best_match_cuboid1(uint64_t cuboid);

   protected:
    std::pair<std::set<uint64_t>, std::map<uint64_t, std::list<uint64_t>>> build_tree_bottom_up();
    uint64_t do_find_best_match_cuboid1(uint64_t cuboid);

    uint64_t get_on_tree_parent(uint64_t child);
    uint64_t get_parent_on_promise(uint64_t child, std::set<uint64_t> coll, int forward);
    std::set<uint64_t> get_on_tree_parents_by_layer(std::set<uint64_t> children) {
        return std::set<uint64_t>(); /*TODO(tatiana)*/
    }
    std::set<uint64_t> get_lowest_cuboids() { return std::set<uint64_t>(); /*TODO(tatiana)*/ }
    std::set<uint64_t> get_on_tree_parents(uint64_t child,
                                           std::set<AggregationGroup*> groups = std::set<AggregationGroup*>());

   private:
    uint64_t max_;
    CubeDesc* cube_desc_;  // not owned
    std::set<uint64_t> all_cuboid_ids_;
    std::map<uint64_t, std::list<uint64_t>> parent2child_;
};

}  // namespace cube
}  // namespace husky
