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

#include "core-cube/cube_instance.hpp"

#include <list>
#include <set>
#include <string>

#include "nlohmann/json.hpp"

#include "core-cube/cuboid/cuboid_scheduler.hpp"
#include "core-cube/cuboid/tree_cuboid_scheduler.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"

namespace husky {
namespace cube {

CubeInstance::CubeInstance(const std::string& cube_name, const json& cube_desc_json, bool tree_scheduler)
    : name_(cube_name) {
    cube_desc_ = std::make_shared<CubeDesc>(cube_desc_json);
    desc_name_ = cube_desc_->get_name();
    if (tree_scheduler) {
        // remember to init_cuboid_scheduler
        cuboid_scheduler_ = std::make_shared<TreeCuboidScheduler>(cube_desc_);
    } else {
        cuboid_scheduler_ = std::make_shared<CuboidScheduler>(cube_desc_);
    }
    cube_desc_->set_cuboid_scheduler(cuboid_scheduler_.get());
}

const std::string& CubeInstance::get_root_fact_table() {
    return cube_desc_->get_model()->get_root_fact_table_ref()->get_table_name();
}

std::set<uint64_t> CubeInstance::get_cuboids() { return cuboid_scheduler_->get_all_cuboid_ids(); }

}  //  namespace cube
}  // namespace husky
