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

include_directories(${PROJECT_SOURCE_DIR} ${HUSKY_EXTERNAL_INCLUDE} ${EXTERNAL_INCLUDE})

set(husky-kylin husky extension-husky-io extension-husky-master) 

### Applications

# WordCountORC
add_executable(WordCountORC wc_orc.cpp)
target_link_libraries(WordCountORC ${husky-kylin})
target_link_libraries(WordCountORC ${EXTERNAL_LIB})
target_link_libraries(WordCountORC ${HUSKY_EXTERNAL_LIB})
set_property(TARGET WordCountORC PROPERTY CXX_STANDARD 14)
add_dependencies(WordCountORC ${EXTERNAL_DEPENDENCIES})

# WordCountPARQUET
add_executable(WordCountPARQUET wc_mr_parquet.cpp)
target_link_libraries(WordCountPARQUET ${husky-kylin})
target_link_libraries(WordCountPARQUET ${HUSKY_EXTERNAL_LIB} ${EXTERNAL_LIB})
set_property(TARGET WordCountPARQUET PROPERTY CXX_STANDARD 14)
add_dependencies(WordCountPARQUET ${EXTERNAL_DEPENDENCIES})

# TestJSON: load json from hdfs and parse
add_executable(TestJSON parse_json.cpp)
target_link_libraries(TestJSON husky)
target_link_libraries(TestJSON ${HUSKY_EXTERNAL_LIB})
add_dependencies(TestJSON ${EXTERNAL_DEPENDENCIES})
