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

include_directories(${PROJECT_SOURCE_DIR} ${HUSKY_EXTERNAL_INCLUDE} ${EXTERNAL_INCLUDE} ${PROJECT_SOURCE_DIR}/husky)

file(GLOB utils-src
  utils.cpp
  )

husky_cache_variable(utils-src ${utils-src})
add_library(husky-kylin-utils SHARED ${utils-src})
target_link_libraries(husky-kylin-utils ${GLOG_LIBRARY})
