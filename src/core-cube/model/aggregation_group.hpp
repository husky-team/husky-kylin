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

#include "core-cube/model/row_key_desc.hpp"
// #include "core-cube/model/select_rule.hpp"

namespace husky {
namespace cube {

class CubeDesc;

class AggregationGroup {
   public:
    AggregationGroup(CubeDesc* cube_desc, RowKeyDesc* row_key_desc) : cube_desc_(cube_desc) {}
    ~AggregationGroup() {}

    inline uint64_t translate_to_on_tree_cuboid(uint64_t cuboid) { return 0; /* TODO(tatiana)*/ }
    inline bool is_on_tree(uint64_t cuboid) { return false; /* TODO(tatiana)*/ }

   private:
    std::vector<std::string> includes_;  // all dims in aggregation groups
    // SelectRule * selectRule;
    CubeDesc* cube_desc_;
};

}  // namespace cube
}  // namespace husky
