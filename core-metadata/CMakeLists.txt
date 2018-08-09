include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/husky)

# files that can be added to the list below
# metadata/datatype/DataType.cpp
# metadata/model/FunctionDesc.cpp
# metadata/model/MeasureDesc.cpp
# metadata/model/ParameterDesc.cpp

file(GLOB core-metadata-src
    metadata/model/ColumnDesc.cpp
    metadata/model/DataModelDesc.cpp
    metadata/model/ModelDimensionDesc.cpp
    metadata/model/TableDesc.cpp
    metadata/model/TableRef.cpp
    metadata/model/TblColRef.cpp
)

add_library(core-metadata STATIC ${core-metadata-src})

add_executable(TestModel metadata/model/testModel.cpp)
target_link_libraries(TestModel core-metadata)
