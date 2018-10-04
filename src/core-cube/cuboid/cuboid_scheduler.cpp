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

#include "core-cube/cuboid/cuboid_scheduler.hpp"

#include <list>
#include <map>
#include <set>
#include <utility>

#include "core-cube/model/aggregation_group.hpp"
#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

CuboidScheduler::CuboidScheduler(CubeDesc* cube_desc) : cube_desc_(cube_desc) {
    int size = cube_desc_->get_row_key()->get_row_key_columns().size();
    max_ = (uint64_t) pow(2, size) - 1;

    std::pair<std::set<uint64_t>, std::map<uint64_t, std::list<uint64_t>>> pair = build_tree_bottom_up();
    this->all_cuboid_ids_ = pair.first;
    this->parent2child_ = pair.second;
}

std::list<uint64_t> CuboidScheduler::get_spanning_cuboid(uint64_t cuboid) {
    if (cuboid > max_ || cuboid < 0) {
        throw "Cuboid" + std::to_string(cuboid) + " is out of scope 0 - " + std::to_string(max_);
    }

    auto pos = parent2child_.find(cuboid);
    if (pos == parent2child_.end()) {
        return std::list<uint64_t>();
    }
    return pos->second;
}

uint64_t CuboidScheduler::find_best_match_cuboid1(uint64_t cuboid) {
    if (is_valid(cuboid)) {
        return cuboid;
    }

    std::list<uint64_t> on_tree_candidates;
    std::list<AggregationGroup*> aggregation_groups = cube_desc_->get_aggregation_groups();
    for (std::list<AggregationGroup*>::iterator it = aggregation_groups.begin(); it != aggregation_groups.end(); ++it) {
        uint64_t candidate = (*it)->translate_to_on_tree_cuboid(cuboid);
        // TODO(tatiana): cannot compare uint64_t with NULL, assume -1 is the false candidate value here
        // if (candidate != NULL) {}
        if (candidate != -1) {
            on_tree_candidates.push_back(candidate);
        }
    }

    if (on_tree_candidates.size() == 0) {
        // FIXME(alice): getBaseCuboidId is not declared
        // return getBaseCuboidId();
        return 0;
    }

    // uint64_t onTreeCandi = min(onTreeCandidates, Cuboid.cuboidSelectComparator);
    // FIXME(alice): on_tree_candi
    uint64_t on_tree_candi = 0;
    if (is_valid(on_tree_candi)) {
        return on_tree_candi;
    }

    return do_find_best_match_cuboid1(on_tree_candi);
}

uint64_t CuboidScheduler::do_find_best_match_cuboid1(uint64_t cuboid) {
    uint64_t parent = get_on_tree_parent(cuboid);
    while (parent > 0) {
        // FIXME(alice): no such function in CubeDesc, I assume it is from this class?
        // std::set<uint64_t> allCuboids = cubeDesc->getAllCuboids();
        std::set<uint64_t> all_cuboids = get_all_cuboid_ids();

        if (all_cuboids.find(parent) != all_cuboids.end()) {
            break;
        }
        parent = get_on_tree_parent(parent);
    }

    if (parent <= 0) {
        throw "Can't find valid parent for Cuboid " + std::to_string(cuboid);
    }
    return parent;
}

uint64_t CuboidScheduler::get_on_tree_parent(uint64_t child) {
    std::set<uint64_t> candidates = get_on_tree_parents(child);
    if (candidates.empty()) {
        return -1;
    }
    // return min(candidates, Cuboid.cuboidSelectComparator);
    return *candidates.begin();
}

std::set<uint64_t> CuboidScheduler::get_on_tree_parents(uint64_t child, std::set<AggregationGroup*> groups) {
    std::set<AggregationGroup*> aggrs;
    std::list<AggregationGroup*> aggregation_groups = cube_desc_->get_aggregation_groups();
    for (std::list<AggregationGroup*>::iterator it = aggregation_groups.begin(); it != aggregation_groups.end(); ++it) {
        if ((*it)->is_on_tree(child)) {
            // FIXME(alice): no agg declared in this scope? I assume *it
            // aggrs.push_back(agg);
            aggrs.insert(*it);
        }
    }

    return get_on_tree_parents(child, aggrs);
}

std::pair<std::set<uint64_t>, std::map<uint64_t, std::list<uint64_t>>> CuboidScheduler::build_tree_bottom_up() {
    /**
     * Collect cuboid from bottom up, considering all factor including black list
     * Build tree steps:
     * 1. Build tree from bottom up considering dim capping
     * 2. Kick out blacked-out cuboids from the tree
     * 3. Make sure all the cuboids have proper "parent", if not add it to the tree.
     *    Direct parent is not necessary, can jump *forward* steps to find in-direct parent.
     *    For example, forward = 1, grandparent can also be the parent. Only if both parent
     *    and grandparent are missing, add grandparent to the tree.
     * @return Cuboid collection
     */
    return std::make_pair(std::set<uint64_t>(), std::map<uint64_t, std::list<uint64_t>>());
}

uint64_t CuboidScheduler::get_parent_on_promise(uint64_t child, std::set<uint64_t> coll, int forward) {
    uint64_t parent = get_on_tree_parent(child);
    if (parent < 0) {
        return -1;
    }

    if (coll.find(parent) != coll.end() || forward == 0) {
        return parent;
    }

    return get_parent_on_promise(parent, coll, forward - 1);
}

}  // namespace cube
}  // namespace husky
