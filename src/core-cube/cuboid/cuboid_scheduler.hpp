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

#include <cmath>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "core-cube/model/aggregation_group.hpp"
#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

class CuboidScheduler : public CuboidSchedulerBase {
   public:
    explicit CuboidScheduler(const std::shared_ptr<CubeDesc>& cube_desc);

    inline std::set<uint64_t> get_all_cuboid_ids() const override { return all_cuboid_ids_; }
    inline int get_cuboid_count() const override { return all_cuboid_ids_.size(); }
    std::vector<uint64_t> get_spanning_cuboid(uint64_t parent_cuboid) const override;

    inline bool is_valid(uint64_t requestCuboid) const override {
        return all_cuboid_ids_.find(requestCuboid) != all_cuboid_ids_.end();
    }

    /** Returns a valid cuboid that best matches the request cuboid. */
    uint64_t find_best_match_cuboid(uint64_t cuboid) const override;

    void init_cuboid_tree(std::vector<uint64_t>& all_cuboid_ids) override;

   protected:
    inline bool check_dim_cap(AggregationGroup* agg, uint64_t cuboid_id) { return true; /*TODO(tatiana)*/ }
    inline std::set<uint64_t> get_lowest_cuboids() const {
        return get_on_tree_parents(0L, cube_desc_->get_aggregation_group_ptrs());
    }

    std::pair<std::set<uint64_t>, std::map<uint64_t, std::vector<uint64_t>>> build_tree_bottom_up();
    uint64_t do_find_best_match_cuboid1(uint64_t cuboid) const;

    uint64_t get_on_tree_parent(uint64_t child) const;
    uint64_t get_parent_on_promise(uint64_t child, const std::set<uint64_t>& coll, int forward);
    std::set<uint64_t> get_on_tree_parents_by_layer(std::set<uint64_t> children);
    std::set<uint64_t> get_on_tree_parents(uint64_t child, const std::vector<AggregationGroup*>& groups) const;
    std::set<uint64_t> get_on_tree_parents(uint64_t child, AggregationGroup* agg) const;
    std::set<uint64_t> get_on_tree_parents(uint64_t child) const;
    bool fill_bit(uint64_t origin, uint64_t other, std::set<uint64_t>* collection) const;

   private:
    uint64_t max_;
    std::set<uint64_t> all_cuboid_ids_;
    std::map<uint64_t, std::vector<uint64_t>> parent2child_;
};

}  // namespace cube
}  // namespace husky
