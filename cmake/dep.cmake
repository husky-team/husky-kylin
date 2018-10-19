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

set(ORC_VERSION "6ef073343c")
set(JSON_VERSION "3.2.0")
set(PARQUET_VERSION "1.4.0")
set(ARROW_VERSION "501d60e918bd4d10c429ab34e0b8e8a87dffb732")

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
  set(ORC_INCLUDE_DIR "${ORC_HOME}/include")
  set(ORC_L1 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L2 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lz4${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L3 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}snappy${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L4 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}orc${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_L5 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}hdfspp_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ORC_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${ORC_HOME} -DCMAKE_INSTALL_LIBDIR=${ORC_HOME}/lib -DBUILD_JAVA=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_CPP_TESTS=OFF)

  if (DEFINED ENV{PROTOBUF_HOME})
    set(PROTOBUF_HOME "$ENV{PROTOBUF_HOME}")
    find_package(Protobuf)
    set(ORC_L0 ${PROTOBUF_STATIC_LIB})
  else()
    set(ORC_L0 "${ORC_HOME}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}protobuf${CMAKE_STATIC_LIBRARY_SUFFIX}")
  endif ()

  find_package(OpenSSL REQUIRED)
  if (OPENSSL_LIBRARIES)
    list(APPEND ORC_L5 ${OPENSSL_LIBRARIES})
  endif()
  find_package(CyrusSASL)
  if (CYRUS_SASL_FOUND)
    # set (SASL_INCLUDE_DIR ${CYRUS_SASL_INCLUDE_DIR})
    # set (SASL_LIBRARIES ${CYRUS_SASL_SHARED_LIB})
    list(APPEND ORC_L5 ${CYRUS_SASL_SHARED_LIB})
  else()
    find_package(GSasl)
    if (GSASL_FOUND)
      # set (SASL_INCLUDE_DIR ${GSASL_INCLUDE_DIR})
      #set (SASL_LIBRARIES ${GSASL_LIBRARIES})
      list(APPEND ORC_L5 ${GSASL_LIBRARIES})
    endif()
  endif()

  ExternalProject_Add(orc_ep
    PREFIX ${THIRDPARTY_DIR}/orc_ep
    URL "https://github.com/TatianaJin/orc/archive/${ORC_VERSION}.tar.gz"
    CMAKE_ARGS ${ORC_CMAKE_ARGS}
    ${THIRDPARTY_LOG_OPTIONS})
  set(ORC_FOUND true)
  list(APPEND EXTERNAL_DEPENDENCIES orc_ep)

  message (STATUS "ORC will be built as a third-party dependency:")
  message (STATUS "  (Headers should be)       ${ORC_INCLUDE_DIR}")
  message (STATUS "  (Library should be)       ${ORC_L0}")
  message (STATUS "  (Library should be)       ${ORC_L1}")
  message (STATUS "  (Library should be)       ${ORC_L2}")
  message (STATUS "  (Library should be)       ${ORC_L3}")
  message (STATUS "  (Library should be)       ${ORC_L4}")
  message (STATUS "  (Library should be)       ${ORC_L5}")
endif ()

if(ORC_FOUND)
  list(APPEND EXTERNAL_INCLUDE "${ORC_INCLUDE_DIR}")
  list(APPEND EXTERNAL_LIB ${ORC_L4} ${ORC_L3} ${ORC_L2} ${ORC_L1} ${ORC_L5} ${ORC_L0})
  list(APPEND EXTERNAL_DEFINITION ${ORC_DEFINITION})
else(ORC_FOUND)
  message (FATAL_ERROR "Could NOT find ORC")
endif(ORC_FOUND)

### END ORC ###

### ARROW for Parquet ###
if (DEFINED ENV{ARROW_HOME})
  find_package(Arrow REQUIRED)
  add_custom_target(arrow_ep)
  set(ARROW_VENDORED FALSE)
else()
  set(ARROW_HOME "${THIRDPARTY_DIR}/arrow")
  set(ENV{ARROW_HOME} ${ARROW_HOME})
  set(ARROW_INCLUDE_DIR "${ARROW_HOME}/include")
  set(ARROW_SHARED_LIB "${ARROW_HOME}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}arrow${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(ARROW_CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${ARROW_HOME}
    -DCMAKE_INSTALL_LIBDIR=${ARROW_HOME}/lib
    -DARROW_JEMALLOC=OFF
    -DARROW_IPC=OFF
    -DARROW_WITH_LZ4=ON
    -DARROW_WITH_ZSTD=ON
    -DARROW_BUILD_TESTS=OFF
    )

  ExternalProject_Add(arrow_ep
    PREFIX ${THIRDPARTY_DIR}/arrow_ep
    URL "https://github.com/apache/arrow/archive/${ARROW_VERSION}.tar.gz"
    SOURCE_SUBDIR "cpp"
    CMAKE_ARGS ${ARROW_CMAKE_ARGS}
    ${THIRDPARTY_LOG_OPTIONS}
    )
  set(ARROW_FOUND true)

  message (STATUS "Arrow will be built as dependency of Parquet:")
  message (STATUS "  (Headers should be)       ${ARROW_INCLUDE_DIR}")
  message (STATUS "  (Library should be)       ${ARROW_SHARED_LIB}")
endif ()

if (ARROW_FOUND)
  list(APPEND EXTERNAL_INCLUDE "${ARROW_INCLUDE_DIR}")
  list(APPEND EXTERNAL_LIB ${ARROW_SHARED_LIB})
endif()
### END ARROW ###

### PARQUET ###

set(PARQUET_DEFINITION "-DWITH_PARQUET")

if (DEFINED ENV{PARQUET_HOME})
  set(PARQUET_HOME "$ENV{PARQUET_HOME}")
endif ()

if (NOT "${PARQUET_HOME}" STREQUAL "")
  find_package(Parquet REQUIRED)
  set(PARQUET_VENDORED FALSE)
else ()
  set(PARQUET_HOME "${THIRDPARTY_DIR}/parquet")
  set(PARQUET_INCLUDE_DIR "${PARQUET_HOME}/include")
  set(PARQUET_LIBRARIES "${PARQUET_HOME}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}parquet${CMAKE_SHARED_LIBRARY_SUFFIX}")
  set(PARQUET_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PARQUET_HOME} -DCMAKE_INSTALL_LIBDIR=${PARQUET_HOME}/lib)

  ExternalProject_Add(parquet_ep
    DEPENDS arrow_ep
    PREFIX ${THIRDPARTY_DIR}/parquet_ep
    URL "https://github.com/apache/parquet-cpp/archive/apache-parquet-cpp-${PARQUET_VERSION}.tar.gz"
    CMAKE_ARGS ${PARQUET_CMAKE_ARGS}
    ${THIRDPARTY_LOG_OPTIONS})
  set(PARQUET_FOUND true)
  list(APPEND EXTERNAL_DEPENDENCIES parquet_ep)

  message (STATUS "Parquet will be built as a third-party dependency:")
  message (STATUS "  (Headers should be)       ${PARQUET_INCLUDE_DIR}")
  message (STATUS "  (Library should be)       ${PARQUET_LIBRARIES}")
endif ()

if(PARQUET_FOUND)
  list(APPEND EXTERNAL_INCLUDE "${PARQUET_INCLUDE_DIR}")
  list(APPEND EXTERNAL_LIB ${PARQUET_LIBRARIES})
  list(APPEND EXTERNAL_DEFINITION ${PARQUET_DEFINITION})
else(PARQUET_FOUND)
  message (WARNING "Could NOT find Parquet")
endif(PARQUET_FOUND)

### END PARQUET ###

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
