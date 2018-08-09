include_directories(${PROJECT_SOURCE_DIR} ${HUSKY_EXTERNAL_INCLUDE})

set(husky-kylin core-metadata husky)

### Reuse gtest in Husky

if(GTEST_SEARCH_PATH)
    message(STATUS "GTest search path ${GTEST_SEARCH_PATH}")
else(GTEST_SEARCH_PATH)
    set(GTEST_SEARCH_PATH "${PROJECT_BINARY_DIR}/lib")
endif(GTEST_SEARCH_PATH)

find_path(GTEST_INCLUDE_DIR NAMES gtest/gtest.h PATHS ${GTEST_SEARCH_PATH} NO_SYSTEM_ENVIRONMENT_PATH)
find_library(GTEST_LIBRARY NAMES gtest PATHS ${GTEST_SEARCH_PATH} NO_SYSTEM_ENVIRONMENT_PATH)
if(GTEST_LIBRARY)
    message(STATUS "Found GTest in search path ${GTEST_SEARCH_PATH}")
else(GTEST_LIBRARY)
    message(ERROR "Cannot find GTest")
    return()
endif(GTEST_LIBRARY)

### Unit Test
file(GLOB_RECURSE UNITTEST_FILES ${PROJECT_SOURCE_DIR}/*_test.cpp) 
add_executable(UnitTest ${UNITTEST_FILES} unittest_main.cpp)
# add_dependencies(UnitTest gtest)
target_link_libraries(UnitTest ${husky-kylin})
target_link_libraries(UnitTest ${HUSKY_EXTERNAL_LIB})
target_link_libraries(UnitTest ${GTEST_LIBRARY})
target_link_libraries(UnitTest ${GMOCK_LIBRARY})

### Functional Test

# TestXXX Template
# add_executable(TestXXX test-xxx.cpp)
# target_link_libraries(TestXXX ${husky})
# target_link_libraries(TestXXX ${HUSKY_EXTERNAL_LIB})
