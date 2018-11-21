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

#include <list>
#include <set>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "core-cube/cuboid/cuboid_scheduler_base.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"

namespace husky {
namespace cube {

using nlohmann::json;

class CubeInstance {
   public:
    CubeInstance(const std::string& cube_name, const json& cube_desc_json, bool tree_scheduler = true);

    inline int get_build_level() const { return cuboid_scheduler_->get_build_level(); }
    inline const std::shared_ptr<CubeDesc>& get_cube_desc() { return cube_desc_; }
    inline const std::shared_ptr<CuboidSchedulerBase>& get_cuboid_scheduler() const { return cuboid_scheduler_; }
    inline const std::string& get_desc_name() const { return desc_name_; }
    inline const std::string& get_name() const { return name_; }
    std::set<uint64_t> get_cuboids();
    inline const std::vector<std::shared_ptr<MeasureDesc>>& get_measures() const { return cube_desc_->get_measures(); }
    const std::string& get_root_fact_table();

    inline void set_name(const std::string& name) { name_ = name; }
    inline void set_desc_name(const std::string& desc_name) { desc_name_ = desc_name; }
    inline void init_cuboid_scheduler(std::vector<uint64_t> all_cuboid_ids = std::vector<uint64_t>()) {
        cuboid_scheduler_->init_cuboid_tree(all_cuboid_ids);
    }

   private:
    std::string name_;  // instance name
    std::string desc_name_;
    std::shared_ptr<CubeDesc> cube_desc_;
    std::shared_ptr<CuboidSchedulerBase> cuboid_scheduler_;
};

}  // namespace cube
}  // namespace husky
