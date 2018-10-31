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
#include <vector>

#include "core-metadata/metadata/model/tbl_col_ref.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

class CuboidSchedulerBase;
class CubeDesc;

class Cuboid {
   public:
    Cuboid(const std::shared_ptr<CubeDesc>& cube_desc, uint64_t original_id, uint64_t valid_id);
    ~Cuboid() {}

    static Cuboid find_cuboid(const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler,
                              const std::set<std::shared_ptr<TblColRef>>& dimensions);
    static Cuboid find_cuboid(CuboidSchedulerBase* cuboid_scheduler,
                              const std::set<std::shared_ptr<TblColRef>>& dimensions);
    static Cuboid find_by_bytes_id(const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler,
                                   const std::vector<unsigned char>& cuboid_id);
    static Cuboid find_by_long_id(const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler, uint64_t cuboid_id);
    static Cuboid find_by_long_id(CuboidSchedulerBase* cuboid_scheduler, uint64_t cuboid_id);
    static uint64_t to_cuboid_id(const std::shared_ptr<CubeDesc>& cube_desc,
                                 const std::set<std::shared_ptr<TblColRef>>& dimensions);
    static uint64_t get_base_cuboid_id(const std::shared_ptr<CubeDesc>& cube);
    static uint64_t get_base_cuboid_id(CubeDesc* cube);
    static Cuboid get_base_cuboid(const std::shared_ptr<CubeDesc>& cube);

    inline const std::shared_ptr<CubeDesc>& get_cube_desc() const { return cube_desc_; }
    inline const std::vector<std::shared_ptr<TblColRef>>& get_columns() const { return dimension_columns_; }
    inline uint64_t get_id() const { return id_; }
    inline const std::vector<unsigned char>& get_bytes() const { return id_bytes_; }
    inline uint64_t get_input_id() const { return input_id_; }

   protected:
    std::vector<std::shared_ptr<TblColRef>> translated_id_to_columns(uint64_t cuboid_id);

   private:
    std::shared_ptr<CubeDesc> cube_desc_;
    uint64_t input_id_;
    uint64_t id_;
    std::vector<unsigned char> id_bytes_;
    std::vector<std::shared_ptr<TblColRef>> dimension_columns_;
};

}  // namespace cube
}  // namespace husky
