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

#include "core-cube/cuboid/cuboid_scheduler.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"

namespace husky {
namespace cube {

class CubeInstance {
   public:
    CubeInstance(const std::string& cube_name, CubeDesc* cube_desc);
    ~CubeInstance() {}

    inline int get_build_level() { return 0; /* TODO(tatiana) */ }
    inline CubeDesc* get_cube_desc() { return cube_desc_; }
    inline std::shared_ptr<CuboidScheduler> get_cuboid_scheduler() const { return cuboid_scheduler_; }
    inline const std::string& get_desc_name() const { return desc_name_; }
    inline const std::string& get_name() const { return name_; }
    std::set<uint64_t> get_cuboids();
    std::list<MeasureDesc*> get_measures();
    std::string get_root_fact_table();

    inline void set_name(const std::string& name) { name_ = name; }
    inline void set_desc_name(const std::string& desc_name) { desc_name_ = desc_name; }

   private:
    std::string name_;  // instance name
    std::string desc_name_;
    CubeDesc* cube_desc_;  // not owned
    std::shared_ptr<CuboidScheduler> cuboid_scheduler_;
};

}  // namespace cube
}  // namespace husky
