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

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core-cube/cuboid/cuboid.hpp"

namespace husky {
namespace cube {

class CubeDesc;

class CuboidSchedulerBase {
   public:
    CuboidSchedulerBase(CubeDesc* cube_desc) : cube_desc_(cube_desc) {}
    // CuboidSchedulerBase() {}  // for test
    ~CuboidSchedulerBase() {}

    /** Returns all cuboids on the tree. */
    virtual std::set<uint64_t> get_all_cuboid_ids() const = 0;

    /** Returns the number of all cuboids. */
    virtual int get_cuboid_count() const = 0;

    /** Returns the child cuboids of a parent. */
    virtual std::vector<uint64_t> get_spanning_cuboid(uint64_t parentCuboid) const = 0;

    /** Returns a valid cuboid that best matches the request cuboid. */
    virtual uint64_t find_best_match_cuboid(uint64_t cuboid) const = 0;

    // ============================================================================

    inline uint64_t get_base_cuboid_id() const { return Cuboid::get_base_cuboid_id(cube_desc_); }

    // inline uint64_t get_base_cuboid_id() { return 31; }  // hard code; for test only

    inline CubeDesc* get_cube_desc() const { return cube_desc_; }

    /** Checks whether a cuboid is valid or not. */
    inline virtual bool is_valid(uint64_t request_cuboid) const {
        std::set<uint64_t> all_cuboid_ids = get_all_cuboid_ids();
        return all_cuboid_ids.find(request_cuboid) != all_cuboid_ids.end();
    }

    /**
     * Get cuboids by layer. It's built from pre-expanding tree.
     *
     * @return layered cuboids
     */
    std::vector<std::vector<uint64_t>> get_cuboids_by_layer();

    /**
     * Get cuboid level count except base cuboid
     */
    inline int get_build_level() { return get_cuboids_by_layer().size() - 1; }

   protected:
    CubeDesc* cube_desc_;  // not owned, this is my owner

   private:
    std::vector<std::vector<uint64_t>> cuboids_by_layer_;
};

}  // namespace cube
}  // namespace husky
