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

include(ExternalProject)

set(EXTERNAL_INCLUDE "")
set(EXTERNAL_LIB "")
set(EXTERNAL_DEFINITION "")
set(THIRDPARTY_DIR ${PROJECT_BINARY_DIR}/third_party)
set(THIRDPARTY_LOG_OPTIONS LOG_CONFIGURE 1 LOG_BUILD 1 LOG_INSTALL 1 LOG_DOWNLOAD 1)

set(ORC_VERSION "1.5.2")
set(JSON_VERSION "3.2.0")

### ORC ###

set(ORC_DEFINITION "-DWITH_ORC")

if (DEFINED ENV{ORC_HOME})
  set(ORC_HOME "$ENV{ORC_HOME}")
endif ()

if (NOT "${ORC_HOME}" STREQUAL "")
  find_package(ORC REQUIRED)
  set(ORC_VENDORED FALSE)
else ()
  set(ORC_HOME "${THIRDPARTY_DIR}/orc")
  set(ORC_INCLUDE_DIR "${ORC_HOME}")
  set(ORC_L0 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L1 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L2 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lz${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L3 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}snappy${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L4 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}orc${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${ORC_HOME} -DBUILD_JAVA=OFF -DBUILD_SHARED_LIBS=OFF)

  ExternalProject_Add(orc_ep
    PREFIX ${THIRDPARTY_DIR}/orc_ep
    URL "http://apache.website-solution.net/orc/orc-${ORC_VERSION}/orc-${ORC_VERSION}.tar.gz"
    CMAKE_ARGS ${ORC_CMAKE_ARGS}
    ${THIRDPARTY_LOG_OPTIONS})
  set(ORC_FOUND true)
  list(APPEND EXTERNAL_DEPENDENCIES orc_ep)
endif ()

if(ORC_FOUND)
  message (STATUS "Found ORC:")
  message (STATUS "  (Headers)       ${ORC_INCLUDE_DIR}")
  message (STATUS "  (Library)       ${ORC_L0}")
  message (STATUS "  (Library)       ${ORC_L1}")
  message (STATUS "  (Library)       ${ORC_L2}") 
  message (STATUS "  (Library)       ${ORC_L3}") 
  message (STATUS "  (Library)       ${ORC_L4}")

  list(APPEND EXTERNAL_INCLUDE "${ORC_INCLUDE_DIR}")
  list(APPEND EXTERNAL_LIB ${ORC_L4} ${ORC_L3} ${ORC_L2} ${ORC_L1} ${ORC_L0})
  list(APPEND EXTERNAL_DEFINITION ${ORC_DEFINITION})
else(ORC_FOUND)
  message (FATAL_ERROR "Could NOT find ORC")
endif(ORC_FOUND)

### END ORC ###

### JSON ###

if (NOT "${JSON_INCLUDE_DIR}" STREQUAL "")
  file(TO_CMAKE_PATH ${JSON_INCLUDE_DIR} _json_path)
endif()

find_path (JSON_INCLUDE_DIR NAMES "nlohmann/json.hpp" HINTS ${_json_path})

if (JSON_INCLUDE_DIR)
  message (STATUS "Found nlohmann/json.hpp:")
  message (STATUS "  (Headers)       ${JSON_INCLUDE_DIR}")
  list(APPEND EXTERNAL_INCLUDE "${JSON_INCLUDE_DIR}")
else()
  ExternalProject_Add(json_ep
    PREFIX ${THIRDPARTY_DIR}/nlohmann
    DOWNLOAD_DIR ${THIRDPARTY_DIR}/nlohmann
    DOWNLOAD_NO_EXTRACT true
    SOURCE_DIR ""
    BINARY_DIR ""
    URL "https://github.com/nlohmann/json/releases/download/v${JSON_VERSION}/json.hpp"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    ${THIRDPARTY_LOG_OPTIONS})
  list(APPEND EXTERNAL_DEPENDENCIES json_ep)
  list(APPEND EXTERNAL_INCLUDE "${THIRDPARTY_DIR}")
endif()

### END JSON ###

include_directories(${EXTERNAL_INCLUDE})
add_definitions(${EXTERNAL_DEFINITION})
