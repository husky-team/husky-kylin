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
set(husky-kylin husky husky-kylin-utils core-cube core-metadata) 

file(GLOB algorithm-src
  cuboid_combiner.cpp
  cubing_by_layer.cpp
  )

husky_cache_variable(algorithm-src ${algorithm-src})
add_library(algorithm STATIC ${algorithm-src})
target_link_libraries(algorithm ${husky-kylin})
target_link_libraries(algorithm ${HUSKY_EXTERNAL_LIB})
