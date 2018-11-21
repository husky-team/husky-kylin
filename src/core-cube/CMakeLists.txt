# Copyright 2018 Husky Team
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/husky ${HUSKY_EXTERNAL_INCLUDE} ${EXTERNAL_INCLUDE})
set(husky-kylin husky extension-husky-io extension-husky-master husky-kylin-utils) 

file(GLOB core-cube-src
  cube_instance.cpp
  cuboid/cuboid.cpp
  cuboid/cuboid_scheduler.cpp
  cuboid/cuboid_scheduler_base.cpp
  cuboid/tree_cuboid_scheduler.cpp
  model/aggregation_group.cpp
  model/cube_desc.cpp
  model/dimension_desc.cpp
  model/row_key_col_desc.cpp
  model/row_key_desc.cpp
  kv/row_key_encoder.cpp
  )

husky_cache_variable(core-cube-src ${core-cube-src})
add_library(core-cube SHARED ${core-cube-src})
target_link_libraries(core-cube ${husky-kylin})
target_link_libraries(core-cube ${HUSKY_EXTERNAL_LIB})
