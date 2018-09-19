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

include_directories(${PROJECT_SOURCE_DIR} ${EXTERNAL_INCLUDE})
set(husky-kylin husky extension-husky-io extension-husky-master) 

### IO ###
file(GLOB io-input-src-files
    io/input/orc_file_splitter.cpp
    io/input/orc_inputformat.cpp
    io/input/parquet_file_splitter.cpp
    io/input/parquet_inputformat.cpp)

husky_cache_variable(io-input-src-files ${io-input-src-files})

add_library(extension-husky-io STATIC ${io-input-src-files})
add_dependencies(extension-husky-io ${EXTERNAL_DEPENDENCIES})
target_link_libraries(extension-husky-io husky)

### MASTER ###
file(GLOB master-src-files
    master/orc_assigner.cpp)

add_library(extension-husky-master STATIC ${master-src-files})
add_dependencies(extension-husky-master ${EXTERNAL_DEPENDENCIES})
target_link_libraries(extension-husky-master husky)

# Master
add_executable(HuskyMaster
  ${PROJECT_SOURCE_DIR}/husky/master/master_main.cpp
  $<TARGET_OBJECTS:husky-master-objs>
  master/orc_assigner.cpp
  master/parquet_assigner.cpp
  )
target_link_libraries(HuskyMaster husky)
target_link_libraries(HuskyMaster ${EXTERNAL_LIB})
target_link_libraries(HuskyMaster ${HUSKY_EXTERNAL_LIB})
set_property(TARGET HuskyMaster PROPERTY CXX_STANDARD 14)
add_dependencies(HuskyMaster ${EXTERNAL_DEPENDENCIES})
