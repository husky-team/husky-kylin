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

file(GLOB core-metadata-src
  dimension/integer_dim_enc.cpp
  metadata/datatype/data_type.cpp
  metadata/model/column_desc.cpp
  metadata/model/data_model_desc.cpp
  metadata/model/function_desc.cpp
  metadata/model/measure_desc.cpp
  metadata/model/model_dimension_desc.cpp
  metadata/model/parameter_desc.cpp
  metadata/model/table_desc.cpp
  metadata/model/table_ref.cpp
  metadata/model/tbl_col_ref.cpp
)

husky_cache_variable(core-metadata-src ${core-metadata-src})
add_library(core-metadata SHARED ${core-metadata-src})
target_link_libraries(core-metadata ${husky-kylin})
target_link_libraries(core-metadata ${HUSKY_EXTERNAL_LIB})
