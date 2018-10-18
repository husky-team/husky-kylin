include_directories(${PROJECT_SOURCE_DIR} ${HUSKY_EXTERNAL_INCLUDE})

set(husky-kylin-core core-cube core-metadata)
set(husky-kylin ${husky-kylin-core} husky)

### Reuse gtest in Husky

get_directory_property(GTEST_LIBRARY DIRECTORY ${PROJECT_SOURCE_DIR}/husky/test DEFINITION GTEST_LIBRARIES GTEST_LIBRARY)
message(STATUS "Will use GTest used in Husky:")
message(STATUS "  (Library should be)       ${GTEST_LIBRARY}")

### Unit Test
file(GLOB_RECURSE UNITTEST_FILES ${PROJECT_SOURCE_DIR}/*_test.cpp)
add_executable(UnitTest ${UNITTEST_FILES} unittest_main.cpp)
add_dependencies(UnitTest gtest)
target_link_libraries(UnitTest ${husky-kylin})
target_link_libraries(UnitTest ${EXTERNAL_LIB})
target_link_libraries(UnitTest ${HUSKY_EXTERNAL_LIB})
target_link_libraries(UnitTest ${GTEST_LIBRARY})
target_link_libraries(UnitTest ${GMOCK_LIBRARY})
file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})


### Functional Test

# TestModel
add_executable(TestModel test_model.cpp)
target_link_libraries(TestModel ${husky-kylin-core})
target_link_libraries(TestModel ${EXTERNAL_LIB})
target_link_libraries(TestModel ${HUSKY_EXTERNAL_LIB})

# TestCube
add_executable(TestCube test_cube.cpp)
target_link_libraries(TestCube ${husky-kylin-core})
target_link_libraries(TestCube ${EXTERNAL_LIB})
target_link_libraries(TestCube ${HUSKY_EXTERNAL_LIB})

# TestXXX Template
# add_executable(TestXXX test-xxx.cpp)
# target_link_libraries(TestXXX ${husky})
# target_link_libraries(TestXXX ${HUSKY_EXTERNAL_LIB})

add_executable(TestTable ../test-table.cpp)
target_link_libraries(TestTable husky)
target_link_libraries(TestTable ${HUSKY_EXTERNAL_LIB})
target_link_libraries(UnitTest ${husky-kylin})
target_link_libraries(UnitTest ${HUSKY_EXTERNAL_LIB})
target_link_libraries(UnitTest ${GTEST_LIBRARY})
target_link_libraries(UnitTest ${GMOCK_LIBRARY})
