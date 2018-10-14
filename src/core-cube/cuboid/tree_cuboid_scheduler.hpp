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

#include <algorithm>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "core-cube/cuboid/cuboid.hpp"
#include "core-cube/cuboid/cuboid_scheduler_base.hpp"

namespace husky {
namespace cube {

class CubeDesc;

/** Do not consider aggregation group, simple cuboid scheduler */
class TreeCuboidScheduler : public CuboidSchedulerBase {
   public:
    class TreeNode {
       public:
        TreeNode(uint64_t cuboid_id, int level) : cuboid_id_(cuboid_id), level_(level) {}
        ~TreeNode() {}

        inline uint64_t get_cuboid_id() const { return cuboid_id_; }
        inline int get_level() const { return level_; }
        inline const std::vector<TreeNode>& get_children() const { return children_; }
        inline std::vector<TreeNode>& get_children() { return children_; }

        inline void add_child(uint64_t child_id, int parent_level) {
            children_.push_back(TreeNode(child_id, parent_level + 1));
        }

       private:
        uint64_t cuboid_id_;
        int level_;
        std::vector<TreeNode> children_;
    };

    class CuboidTree {
       public:
        CuboidTree() {}
        ~CuboidTree() {}

        /** Has side effect on the input all_cuboid_ids */
        static CuboidTree create_from_cuboids(std::vector<uint64_t>& all_cuboid_ids);

        std::set<uint64_t> get_all_cuboid_ids() const;
        std::vector<uint64_t> get_spanning_cuboid(uint64_t cuboid_id) const;  // get all children of cuboidId
        uint64_t find_best_match_cuboid(uint64_t cuboid_id) const;            // find best parent
        bool is_valid(uint64_t cuboid_id) const;                              // if index contains key (cuboidId)
        int get_cuboid_count(uint64_t cuboid_id) const;  // get # of all tree cuboids taking cuboid_id as root

       private:
        void set_root(uint64_t basic_cuboid_id);
        void build_index();
        void add_cuboid(uint64_t cuboid_id);
        TreeNode* find_best_parent(uint64_t cuboid_id) const;
        TreeNode* do_find_best_parent(uint64_t cuboid_id, TreeNode* parent_cuboid) const;
        bool can_derive(uint64_t cuboid_id, uint64_t parent_cuboid) const;

        int tree_levels_;
        std::shared_ptr<TreeNode> root_;
        std::map<uint64_t, TreeNode*> index_;
    };

    explicit TreeCuboidScheduler(std::shared_ptr<CubeDesc> cube_desc, std::vector<uint64_t> & all_cuboid_ids)
            :CuboidSchedulerBase(cube_desc), cuboid_tree_(CuboidTree::create_from_cuboids(all_cuboid_ids)){}
    // explicit TreeCuboidScheduler(std::vector<uint64_t>& all_cuboid_ids)
        // : cuboid_tree_(CuboidTree::create_from_cuboids(all_cuboid_ids)) {}
    TreeCuboidScheduler(std::shared_ptr<CubeDesc> cube_desc):CuboidSchedulerBase(cube_desc){}
    ~TreeCuboidScheduler() {}

    inline std::set<uint64_t> get_all_cuboid_ids() const override { return cuboid_tree_.get_all_cuboid_ids(); }
    inline int get_cuboid_count() const { return
    cuboid_tree_.get_cuboid_count(Cuboid::get_base_cuboid_id(cube_desc_)); }
    // inline int get_cuboid_count() const override {
    //     return cuboid_tree_.get_cuboid_count(31);
    // }  // hard code - [base cuboids id:                                                                                           ] 11111(binary)
    inline std::vector<uint64_t> get_spanning_cuboid(uint64_t cuboid_id) const override {
        return cuboid_tree_.get_spanning_cuboid(cuboid_id);
    }
    inline uint64_t find_best_match_cuboid(uint64_t cuboid_id) const override {
        return cuboid_tree_.find_best_match_cuboid(cuboid_id);
    }
    inline bool is_valid(uint64_t request_cuboid) const override { return cuboid_tree_.is_valid(request_cuboid); }

   private:
    CuboidTree cuboid_tree_;
};

}  // namespace cube
}  // namespace husky
