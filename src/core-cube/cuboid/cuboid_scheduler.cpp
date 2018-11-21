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

#include <algorithm>
#include <deque>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "core-cube/model/aggregation_group.hpp"
#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

CuboidScheduler::CuboidScheduler(const std::shared_ptr<CubeDesc>& cube_desc) : CuboidSchedulerBase(cube_desc) {
    int size = cube_desc_->get_row_key()->get_row_key_columns().size();
    max_ = pow(2, size) - 1;
}

std::vector<uint64_t> CuboidScheduler::get_spanning_cuboid(uint64_t parent_cuboid) const {
    if (parent_cuboid > max_) {
        throw "Cuboid " + std::to_string(parent_cuboid) + " is out of scope 0 - " + std::to_string(max_);
    }

    auto pos = parent2child_.find(parent_cuboid);
    if (pos == parent2child_.end()) {
        return std::vector<uint64_t>();
    }
    return pos->second;
}

uint64_t CuboidScheduler::find_best_match_cuboid(uint64_t cuboid) const {
    if (is_valid(cuboid)) {
        return cuboid;
    }

    std::vector<uint64_t> on_tree_candidates;
    auto& aggregation_groups = cube_desc_->get_aggregation_groups();
    for (auto& agg_group : aggregation_groups) {
        uint64_t candidate = agg_group->translate_to_on_tree_cuboid(cuboid);
        on_tree_candidates.push_back(candidate);
    }

    if (on_tree_candidates.empty()) {
        return get_base_cuboid_id();
    }

    auto min =
        *std::min_element(on_tree_candidates.begin(), on_tree_candidates.end(), Cuboid::cuboid_select_comparator);
    if (is_valid(min)) {
        return min;
    }

    return do_find_best_match_cuboid1(min);
}

void CuboidScheduler::init_cuboid_tree(std::vector<uint64_t>& all_cuboid_ids) {
    auto pair = build_tree_bottom_up();
    all_cuboid_ids_ = pair.first;
    parent2child_ = pair.second;
}

uint64_t CuboidScheduler::do_find_best_match_cuboid1(uint64_t cuboid) const {
    uint64_t parent = get_on_tree_parent(cuboid);
    while (parent > 0) {
        if (is_valid(parent)) {
            break;
        }
        parent = get_on_tree_parent(parent);
    }

    if (parent == 0) {
        throw "Can't find valid parent for Cuboid " + std::to_string(cuboid);
    }
    return parent;
}

uint64_t CuboidScheduler::get_on_tree_parent(uint64_t child) const {
    std::set<uint64_t> candidates = get_on_tree_parents(child);
    if (candidates.empty()) {
        return 0;
    }
    return *std::min_element(candidates.begin(), candidates.end(), Cuboid::cuboid_select_comparator);
}

std::set<uint64_t> CuboidScheduler::get_on_tree_parents(uint64_t child) const {
    auto& agg_groups = cube_desc_->get_aggregation_groups();
    std::vector<AggregationGroup*> aggrs;
    for (auto& agg_group : agg_groups) {
        if (agg_group->is_on_tree(child)) {
            aggrs.push_back(agg_group.get());
        }
    }

    return get_on_tree_parents(child, aggrs);
}

std::set<uint64_t> CuboidScheduler::get_on_tree_parents(uint64_t child,
                                                        const std::vector<AggregationGroup*>& groups) const {
    std::set<uint64_t> candidates;
    if (child == Cuboid::get_base_cuboid_id(cube_desc_)) {
        return candidates;
    }
    for (auto agg : groups) {
        if (child == agg->get_partial_cube_full_mask()) {
            candidates.insert(Cuboid::get_base_cuboid_id(cube_desc_));
            return candidates;
        }
        auto partial = get_on_tree_parents(child, agg);
        candidates.insert(partial.begin(), partial.end());
    }
    return candidates;
}

std::set<uint64_t> CuboidScheduler::get_on_tree_parents(uint64_t child, AggregationGroup* agg) const {
    std::set<uint64_t> parent_candidate;

    uint64_t tmp_child = child;
    if (tmp_child == agg->get_partial_cube_full_mask()) {
        return parent_candidate;
    }

    if (agg->get_mandatory_column_mask() != 0L) {
        if (agg->is_mandatory_only_valid()) {
            if (fill_bit(tmp_child, agg->get_mandatory_column_mask(), &parent_candidate)) {
                return parent_candidate;
            }
        } else {
            tmp_child |= agg->get_mandatory_column_mask();
        }
    }

    for (auto normal : agg->get_normal_dims()) {
        fill_bit(tmp_child, normal, &parent_candidate);
    }

    // TODO(tatiana)
    // for (Long joint : agg.getJoints()) {
    //     fillBit(tmpChild, joint, parentCandidate);
    // }

    // for (auto& hierarchy : agg.getHierarchyMasks()) {
    //     for (long mask : hierarchy.allMasks) {
    //         if (fillBit(tmpChild, mask, parentCandidate)) {
    //             break;
    //         }
    //     }
    // }

    return parent_candidate;
}

bool CuboidScheduler::fill_bit(uint64_t origin, uint64_t other, std::set<uint64_t>* collection) const {
    if ((origin & other) != other) {
        collection->insert(origin | other);
        return true;
    }
    return false;
}

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
std::pair<std::set<uint64_t>, std::map<uint64_t, std::vector<uint64_t>>> CuboidScheduler::build_tree_bottom_up() {
    std::set<uint64_t> cuboid_ids;
    std::map<uint64_t, std::vector<uint64_t>> parent2child;

    // build tree structure
    auto children = get_lowest_cuboids();
    uint64_t max_combination = ~0Lu;  // TODO(tatiana): CubeAggrGroupMaxCombination * 10

    while (!children.empty()) {
        if (cuboid_ids.size() > max_combination) {
            throw "Too many cuboids for the cube. Cuboid combination reached " + std::to_string(cuboid_ids.size()) +
                " and limit is " + std::to_string(max_combination) + ". Abort calculation.";
        }
        cuboid_ids.insert(children.begin(), children.end());
        children = get_on_tree_parents_by_layer(children);
    }
    cuboid_ids.insert(Cuboid::get_base_cuboid_id(cube_desc_));

    // TODO(tatiana): consider blacklist

    // fill padding cuboids
    std::deque<uint64_t> scan;
    scan.insert(scan.end(), cuboid_ids.begin(), cuboid_ids.end());
    int forward = cube_desc_->get_parent_forward();
    while (!scan.empty()) {
        auto current = scan.front();
        scan.pop_front();
        auto parent = get_parent_on_promise(current, cuboid_ids, forward);

        if (parent > 0) {
            if (cuboid_ids.count(parent) == 0) {
                cuboid_ids.insert(parent);
                scan.push_back(parent);
            }
            parent2child[parent].push_back(current);
        }
    }

    return std::make_pair(std::move(cuboid_ids), std::move(parent2child));
}

/**
 * jump as far as forward steps to get parent if not already listed in coll
 */
uint64_t CuboidScheduler::get_parent_on_promise(uint64_t child, const std::set<uint64_t>& coll, int forward) {
    uint64_t parent = get_on_tree_parent(child);
    if (parent == 0) {
        return 0;
    }
    if (coll.find(parent) != coll.end() || forward == 0) {
        return parent;
    }
    return get_parent_on_promise(parent, coll, forward - 1);
}

std::set<uint64_t> CuboidScheduler::get_on_tree_parents_by_layer(std::set<uint64_t> children) {
    std::set<uint64_t> parents;
    std::set<uint64_t> ret;
    for (auto child : children) {
        auto set = get_on_tree_parents(child);
        parents.insert(set.begin(), set.end());
    }
    for (auto candidate : parents) {
        if (candidate == Cuboid::get_base_cuboid_id(cube_desc_)) {
            ret.insert(candidate);
        }
        for (auto& agg : cube_desc_->get_aggregation_groups()) {
            if (agg->is_on_tree(candidate) && check_dim_cap(agg.get(), candidate)) {
                ret.insert(candidate);
            }
        }
    }
    return ret;
}

}  // namespace cube
}  // namespace husky
