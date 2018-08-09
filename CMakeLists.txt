project(husky-kylin)

cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

### Third-party
add_subdirectory(husky)
include_directories(husky third_party) #${HUSKY_EXTERNAL_INCLUDE})
#add_definitions(${HUSKY_EXTERNAL_DEFINITION})

### Directories
add_subdirectory(core-cube)
add_subdirectory(core-metadata)
add_subdirectory(test)

### Executables
add_executable(TestJSON parse_json.cpp)
target_link_libraries(TestJSON husky)
target_link_libraries(TestJSON ${HUSKY_EXTERNAL_LIB})
