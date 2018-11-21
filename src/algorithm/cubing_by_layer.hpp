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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "core/engine.hpp"

#include "algorithm/cuboid_kv.hpp"
#include "core-cube/cuboid/cuboid_scheduler_base.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "table/table.hpp"
#include "utils/hash.hpp"

namespace husky {
namespace cube {

using table::Table;

class CubingByLayer {
   public:
    static void save_to_hdfs(ObjList<CuboidKV>& records, const std::string& output_path, int level);
    static std::string get_cuboid_output_path(const std::string& output_path, int level);

    CubingByLayer(const std::shared_ptr<CubeDesc>& cube_desc,
                  const std::shared_ptr<CuboidSchedulerBase>& cuboid_scheduler, const std::string& output_path)
        : cube_desc_(cube_desc), cuboid_scheduler_(cuboid_scheduler), output_path_(output_path) {}

    void build_cube(Table& table);

   protected:
    auto& build_base_cuboid(Table& table);

    void build_nd_cuboid(ObjList<CuboidKV>& base_cuboid_objlist);

   private:
    std::shared_ptr<CubeDesc> cube_desc_;
    std::shared_ptr<CuboidSchedulerBase> cuboid_scheduler_;
    std::string output_path_ = "";

    std::map<uint64_t, std::shared_ptr<Cuboid>> cuboid_cache_;
};

}  // namespace cube
}  // namespace husky
