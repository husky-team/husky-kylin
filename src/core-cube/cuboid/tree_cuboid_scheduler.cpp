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

#include "core-cube/cuboid/tree_cuboid_scheduler.hpp"

#include <algorithm>
#include <queue>
#include <set>
#include <vector>

#include "glog/logging.h"

#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

TreeCuboidScheduler::CuboidTree TreeCuboidScheduler::CuboidTree::create_from_cuboids(
    std::vector<uint64_t>& all_cuboid_ids) {
    // sort the cuboid ids in descending order, so that don't need to adjust
    // the cuboid tree when adding cuboid id to the tree.
    std::sort(all_cuboid_ids.begin(), all_cuboid_ids.end(), [](const auto& lhs, const auto& rhs) { return lhs > rhs; });
    uint64_t basic_cuboid_id = all_cuboid_ids[0];
    CuboidTree cuboid_tree;
    cuboid_tree.set_root(basic_cuboid_id);

    for (auto const& cuboid_id : all_cuboid_ids) {
        cuboid_tree.add_cuboid(cuboid_id);
    }
    cuboid_tree.build_index();
    return cuboid_tree;
}

std::set<uint64_t> TreeCuboidScheduler::CuboidTree::get_all_cuboid_ids() const {
    std::set<uint64_t> keys;
    for (auto const& cuboid_id : index_) {
        keys.insert(cuboid_id.first);
    }
    return keys;
}

std::vector<uint64_t> TreeCuboidScheduler::CuboidTree::get_spanning_cuboid(uint64_t cuboid_id) const {
    auto pos = index_.find(cuboid_id);
    CHECK_EQ(true, pos != index_.end()) << "[CuboidTree] Cannot find cuboid with id " << cuboid_id;
    TreeNode* node = pos->second;

    std::vector<uint64_t> result;
    for (auto const& child : node->get_children()) {
        result.push_back(child.get_cuboid_id());
    }
    return result;
}

uint64_t TreeCuboidScheduler::CuboidTree::find_best_match_cuboid(uint64_t cuboid_id) const {
    // exactly match
    if (is_valid(cuboid_id)) {
        return cuboid_id;
    }

    return find_best_parent(cuboid_id)->get_cuboid_id();
}

bool TreeCuboidScheduler::CuboidTree::is_valid(uint64_t cuboid_id) const {
    return index_.find(cuboid_id) != index_.end();
}

int TreeCuboidScheduler::CuboidTree::get_cuboid_count(uint64_t cuboid_id) const {
    int r = 1;
    for (uint64_t child : get_spanning_cuboid(cuboid_id)) {
        r += get_cuboid_count(child);
    }
    return r;
}

void TreeCuboidScheduler::CuboidTree::set_root(uint64_t basic_cuboid_id) {
    root_ = std::make_shared<TreeNode>(basic_cuboid_id, 0);
    tree_levels_ = 0;
}

void TreeCuboidScheduler::CuboidTree::build_index() {
    std::queue<TreeNode*> tree_queue;
    tree_queue.push(root_.get());
    while (!tree_queue.empty()) {
        TreeNode* node = tree_queue.front();
        index_.insert(std::make_pair(node->get_cuboid_id(), node));
        for (auto& child : node->get_children()) {
            tree_queue.push(&child);
        }
        tree_queue.pop();
    }
}

void TreeCuboidScheduler::CuboidTree::add_cuboid(uint64_t cuboid_id) {
    TreeNode* parent = find_best_parent(cuboid_id);
    if (parent != nullptr && parent->get_cuboid_id() != cuboid_id) {
        parent->add_child(cuboid_id, parent->get_level());
        tree_levels_ = std::max(tree_levels_, parent->get_level() + 1);
    }
}

TreeCuboidScheduler::TreeNode* TreeCuboidScheduler::CuboidTree::find_best_parent(uint64_t cuboid_id) const {
    TreeNode* best_parent = do_find_best_parent(cuboid_id, root_.get());
    if (best_parent == nullptr) {
        LOG(ERROR) << "[CuboidTree] cannot find a valid parent for cuboid " << cuboid_id;
        // throw some exception
    }
    return best_parent;
}

TreeCuboidScheduler::TreeNode* TreeCuboidScheduler::CuboidTree::do_find_best_parent(uint64_t cuboid_id,
                                                                                    TreeNode* parent_cuboid) const {
    if (!can_derive(cuboid_id, parent_cuboid->get_cuboid_id())) {
        return nullptr;
    }

    std::vector<TreeNode*> candidates;
    std::vector<TreeNode>& children = parent_cuboid->get_children();
    for (auto& child : children) {
        TreeNode* candidate = do_find_best_parent(cuboid_id, &child);
        if (candidate != nullptr) {
            candidates.push_back(candidate);
        }
    }
    if (candidates.empty()) {
        candidates.push_back(parent_cuboid);
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const auto& lhs, const auto& rhs) { return lhs->get_cuboid_id() < rhs->get_cuboid_id(); });
    return candidates[0];  // min candidate
}

bool TreeCuboidScheduler::CuboidTree::can_derive(uint64_t cuboid_id, uint64_t parent_cuboid) const {
    return (cuboid_id & ~parent_cuboid) == 0;
}

}  // namespace cube
}  // namespace husky
