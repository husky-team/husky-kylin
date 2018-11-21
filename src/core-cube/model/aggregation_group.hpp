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

#include <string>
#include <vector>

#include "glog/logging.h"
#include "nlohmann/json.hpp"

#include "core-cube/model/row_key_desc.hpp"
// #include "core-cube/model/select_rule.hpp"

namespace husky {
namespace cube {

using nlohmann::json;
class CubeDesc;

class AggregationGroup {
   public:
    explicit AggregationGroup(const json& agg_json);

    void init(CubeDesc* cube_desc, RowKeyDesc* row_key_desc);

    inline uint64_t get_partial_cube_full_mask() const { return partial_cube_full_mask_; }
    inline uint64_t get_mandatory_column_mask() const { return mandatory_column_mask_; }
    inline uint64_t get_normal_dims_mask() const { return normal_dims_mask_; }
    inline const auto& get_normal_dims() const { return normal_dims_; }
    inline bool is_mandatory_only_valid() const { return is_mandatory_only_valid_; }
    uint64_t translate_to_on_tree_cuboid(uint64_t cuboid_id);
    bool is_on_tree(uint64_t cuboid_id);

   private:
    void build_partial_cube_full_mask(RowKeyDesc* row_key_desc);
    void build_mandatory_column_mask(RowKeyDesc* row_key_desc);
    void build_normal_dims_mask();
    void normalize_column_names();
    void normalize_column_names(std::vector<std::string>* names) const;

    bool check_mandatory_columns(uint64_t cuboid_id);

    std::vector<uint64_t> bits(uint64_t x) {
        std::vector<uint64_t> ret;
        auto l = x;
        while (l != 0) {
            auto bit = l & (~l + 1);
            ret.push_back(bit);
            l ^= bit;
        }
        return ret;
    }

    /* from json */
    std::vector<std::string> includes_;  // all dims in aggregation groups
    // Select Rules
    std::vector<std::string> mandatory_dims_;
    std::vector<std::vector<std::string>> hierarchy_dims_;  // not supported yet
    std::vector<std::vector<std::string>> joint_dims_;      // not supported yet
    int dim_cap_ = 0;                                       // not supported yet

    /* computed*/
    CubeDesc* cube_desc_;
    uint64_t partial_cube_full_mask_;
    uint64_t mandatory_column_mask_;
    bool is_mandatory_only_valid_ = true;  // TODO(tatiana)
    uint64_t normal_dims_mask_;
    std::vector<uint64_t> normal_dims_;  // each value is a single dim
    // std::vector<HierarchyMask> hierarchyMasks;
    // std::vector<uint64_t> joints;  // each value is a group
    // uint64_t jointDimsMask;
    // uint64_t hierarchyDimsMask;
    // std::map<uint64_t, uint64_t> dim2JointMap;
};

}  // namespace cube
}  // namespace husky
