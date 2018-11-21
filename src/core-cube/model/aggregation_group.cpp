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

#include "core-cube/model/aggregation_group.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "glog/logging.h"

#include "core-cube/model/cube_desc.hpp"
#include "core-cube/model/row_key_desc.hpp"

namespace husky {
namespace cube {

AggregationGroup::AggregationGroup(const json& agg_json) {
    // includes
    auto& j_includes = agg_json["includes"];
    includes_.reserve(j_includes.size());
    for (auto& j_include : j_includes) {
        includes_.push_back(j_include.get<std::string>());
    }

    // mandatory dims
    auto& j_mandatory_dims = agg_json["select_rule"]["mandatory_dims"];
    mandatory_dims_.reserve(j_mandatory_dims.size());
    for (auto& j_mandatory_dim : j_mandatory_dims) {
        mandatory_dims_.push_back(j_mandatory_dim.get<std::string>());
    }
}

bool AggregationGroup::is_on_tree(uint64_t cuboid_id) {
    if (cuboid_id == 0) {
        return false;  // cuboid must be greater than 0
    }
    if ((cuboid_id & ~partial_cube_full_mask_) != 0) {
        return false;  // a cuboid's parent within agg is at most partialCubeFullMask
    }

    return check_mandatory_columns(cuboid_id);
    // TODO(tatiana) && checkHierarchy(cuboid_id) && checkJoint(cuboid_id);
}

uint64_t AggregationGroup::translate_to_on_tree_cuboid(uint64_t cuboid_id) {
    if ((cuboid_id & ~get_partial_cube_full_mask()) > 0) {
        // the partial cube might not contain all required dims
        return 0;
    }

    // add mandantory
    cuboid_id |= get_mandatory_column_mask();

    /* TODO(tatiana)
    // add hierarchy
    // for (HierarchyMask hierarchyMask : getHierarchyMasks()) {
    //     long fullMask = hierarchyMask.fullMask;
    //     long intersect = cuboid_id & fullMask;
    //     if (intersect != 0 && intersect != fullMask) {
    //         boolean startToFill = false;
    //         for (int i = hierarchyMask.dims.length - 1; i >= 0; i--) {
    //             if (startToFill) {
    //                 cuboid_id |= hierarchyMask.dims[i];
    //             } else {
    //                 if ((cuboid_id & hierarchyMask.dims[i]) != 0) {
    //                     startToFill = true;
    //                     cuboid_id |= hierarchyMask.dims[i];
    //                 }
    //             }
    //         }
    //     }
    // }

    // add joint dims
    // for (Long joint : getJoints()) {
    //     if (((cuboid_id | joint) != cuboid_id) && ((cuboid_id & ~joint) != cuboid_id)) {
    //         cuboid_id = cuboid_id | joint;
    //     }
    // }
    */

    if (!is_on_tree(cuboid_id)) {
        /*
          // no column, add one column
          long nonJointDims = removeBits((getPartialCubeFullMask() ^ getMandatoryColumnMask()), getJoints());
          if (nonJointDims != 0) {
              long nonJointNonHierarchy = removeBits(
                  nonJointDims, Collections2.transform(getHierarchyMasks(), new Function<HierarchyMask, Long>() {
                      @Override public Long apply(HierarchyMask input) {
                          return input.fullMask;
                      }
                  }));
              if (nonJointNonHierarchy != 0) {
                  // there exists dim that does not belong to any joint or any hierarchy, that's perfect
                  return cuboid_id | Long.lowestOneBit(nonJointNonHierarchy);
              } else {
                  // choose from a hierarchy that does not intersect with any joint dim, only check level 1
                  long allJointDims = getJointDimsMask();
                  for (HierarchyMask hierarchyMask : getHierarchyMasks()) {
                      long dim = hierarchyMask.allMasks[0];
                      if ((dim & allJointDims) == 0) {
                          return cuboid_id | dim;
                      }
                  }
              }
          }
          if (getJoints().size() > 0) {
              cuboid_id = cuboid_id | Collections.min(getJoints(), Cuboid.cuboidSelectComparator);
          }
          if (!is_on_tree(cuboid_id)) {
              // kylin.cube.aggrgroup.is-mandatory-only-valid can be false
              return 0;
          }
          */
    }
    return cuboid_id;
}

void AggregationGroup::build_partial_cube_full_mask(RowKeyDesc* row_key_desc) {
    CHECK_EQ(includes_.empty(), false);
    partial_cube_full_mask_ = 0L;
    for (auto& dim : includes_) {
        auto h_col = cube_desc_->get_model()->find_column(dim);
        int index = row_key_desc->get_column_bit_index(h_col);
        partial_cube_full_mask_ |= (1L << index);
    }
}

void AggregationGroup::build_mandatory_column_mask(RowKeyDesc* row_key_desc) {
    mandatory_column_mask_ = 0L;
    if (mandatory_dims_.empty()) {
        return;
    }
    for (auto& dim : mandatory_dims_) {
        auto h_col = cube_desc_->get_model()->find_column(dim);
        int index = row_key_desc->get_column_bit_index(h_col);
        mandatory_column_mask_ |= (1L << index);
    }
}

void AggregationGroup::build_normal_dims_mask() {
    // no joint, no hierarchy, no mandatory
    uint64_t normal_dims_mask_ = partial_cube_full_mask_ & ~mandatory_column_mask_;
    /* TODO(tatiana)
    normal_dims_mask_  &= ~this.jointDimsMask;
    for (HierarchyMask hierarchyMask : this.hierarchyMasks) {
        normal_dims_mask_  &= ~hierarchyMask.fullMask;
    }
    */

    normal_dims_ = bits(normal_dims_mask_);
}

void AggregationGroup::init(CubeDesc* cube_desc, RowKeyDesc* row_key_desc) {
    CHECK_EQ(includes_.empty(), false);
    cube_desc_ = cube_desc;

    build_partial_cube_full_mask(row_key_desc);
    build_mandatory_column_mask(row_key_desc);
    // TODO(tatiana)
    // buildJointColumnMask(rowKeyDesc);
    // buildJointDimsMask();
    // buildHierarchyMasks(rowKeyDesc);
    // buildHierarchyDimsMask();
    build_normal_dims_mask();
}

void AggregationGroup::normalize_column_names() {
    normalize_column_names(&includes_);
    normalize_column_names(&mandatory_dims_);
    for (auto& dims : hierarchy_dims_) {
        normalize_column_names(&dims);
    }
    for (auto& dims : joint_dims_) {
        normalize_column_names(&dims);
    }
}

void AggregationGroup::normalize_column_names(std::vector<std::string>* names) const {
    if (names == nullptr)
        return;

    for (auto& name : *names) {
        auto col = cube_desc_->get_model()->find_column(name);
        name = col->get_identity();
    }

    // check no duplicate
    auto check = *names;
    std::sort(check.begin(), check.end());
    auto last = std::unique(check.begin(), check.end());
    if (last != check.end()) {
        throw "Columns in aggrgroup must not contain duplication: ";
    }
}

bool AggregationGroup::check_mandatory_columns(uint64_t cuboid_id) {
    if ((cuboid_id & mandatory_column_mask_) != mandatory_column_mask_) {
        return false;
    }
    // base cuboid is always valid
    if (cuboid_id == Cuboid::get_base_cuboid_id(cube_desc_)) {
        return true;
    }

    // cuboid with only mandatory columns maybe valid
    return is_mandatory_only_valid_ || (cuboid_id & ~mandatory_column_mask_) != 0;
}

}  // namespace cube
}  // namespace husky
