// Copyright 2018 Husky Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>
#include <string>
#include <vector>

#include "glog/logging.h"

#include "core-cube/cube_instance.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-cube/model/dimension_desc.hpp"
#include "core-cube/model/row_key_col_desc.hpp"
#include "core-cube/model/row_key_desc.hpp"

#include "core-metadata/metadata/model/column_desc.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/function_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"
#include "core-metadata/metadata/model/model_dimension_desc.hpp"
#include "core-metadata/metadata/model/table_desc.hpp"
#include "core-metadata/metadata/model/table_ref.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

using husky::cube::ColumnDesc;
using husky::cube::DataModelDesc;
using husky::cube::ModelDimensionDesc;
using husky::cube::TableDesc;
using husky::cube::TableRef;
using husky::cube::TblColRef;
using husky::cube::MeasureDesc;
using husky::cube::FunctionDesc;

using husky::cube::CubeInstance;
using husky::cube::CubeDesc;
using husky::cube::DimensionDesc;
using husky::cube::RowKeyDesc;
using husky::cube::RowKeyColDesc;

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    std::string cube_instance_name = "test_cube";
    std::string cube_desc_json_path = "./resources/cube_desc.json";
    std::string model_json_path = "./resources/model.json";
    std::string table_json_path = "./resources/table.json";

    // FIXME(tatiana): json instead of path
    CubeInstance cube_instance = CubeInstance(cube_instance_name, cube_desc_json_path);
    std::shared_ptr<CubeDesc> cube_desc = cube_instance.get_cube_desc();
    cube_desc->init(model_json_path, table_json_path);
    std::shared_ptr<DataModelDesc> model = cube_desc->get_model();

    /* Test CubeInstance*/
    LOG(INFO) << "Cube Instance name: " << cube_instance.get_name();

    // use cuboid scheduler in this way
    std::vector<uint64_t> ALL_CUBOID_IDS = {
        31L,                          // 11111
        30L, 27L, 15L,                // 11110, 11011, 01111
        28L, 26L, 22L, 14L, 11L,      // 11100, 11010, 10110, 01110, 01011
        20L, 18L, 17L, 6L,  5L,  3L,  // 10100, 10010, 10001, 00110, 00101, 00011
        16L, 8L,  4L,  2L,  1L,       // 10000, 01000, 00100, 00010, 00001
        0L                            // 00000
    };
    cube_instance.init_cuboid_scheduler(ALL_CUBOID_IDS);
    /* End of Test CubeInstance*/

    /* Test CubeDesc*/
    LOG(INFO) << "Cube name: " << cube_desc->get_name();

    // Test Dimensions (Output by TableRef and TblColRef)
    const std::vector<std::shared_ptr<DimensionDesc>> dimensions = cube_desc->get_dimensions();
    LOG(INFO) << "Number of Dimensions in cube: " << dimensions.size();
    LOG(INFO) << "First dimension:  Column " << dimensions[0]->get_column_ref()->get_name() << " in Table "
              << dimensions[0]->get_table_ref()->get_alias();
    LOG(INFO) << "First dimension:  Column " << dimensions[1]->get_column_ref()->get_name() << " in Table "
              << dimensions[1]->get_table_ref()->get_alias();

    // Test Measures
    const std::vector<std::shared_ptr<MeasureDesc>> measures = cube_desc->get_measures();
    LOG(INFO) << "Number of Measures: " << measures.size();
    LOG(INFO) << "First measures: " << measures[0]->get_name() << " using function "
              << measures[0]->get_function()->get_expression();
    LOG(INFO) << "Second measures: " << measures[1]->get_name() << " using function "
              << measures[1]->get_function()->get_expression();

    // Test RowKey
    std::shared_ptr<RowKeyDesc> rowkey = cube_desc->get_row_key();
    // LOG(INFO) << "Num of rowkey columns in cube: " << cube_desc->get_row_key_columns().size();
    LOG(INFO) << "Num of rowkey columns: " << rowkey->get_row_key_columns().size();
    LOG(INFO) << "Full Mask(Base Cuboid Id): " << rowkey->get_full_mask();
    /* End of Test CubeDesc*/

    /* Test DataModel */
    LOG(INFO) << "The model name: " << model->get_name();

    std::string table = "KYLIN_SALES";
    std::shared_ptr<TableRef> find_table = model->find_table(table);
    LOG(INFO) << "Found table name: " << find_table->get_alias();

    std::string col1 = "PART_DT";
    std::shared_ptr<TblColRef> find_tbl_col = model->find_column(table, col1);
    LOG(INFO) << "Found column by table: " << find_tbl_col->get_name();

    std::string col2 = "OPS_USER_ID";
    std::shared_ptr<TblColRef> find_col = model->find_column(col2);
    LOG(INFO) << "Found column: " << find_col->get_name();

    // Test TableRef
    auto root_fact_table_ref = model->get_root_fact_table_ref();
    LOG(INFO) << "The root fact table name: " << root_fact_table_ref->get_table_name();
    LOG(INFO) << "The root fact table alias: " << root_fact_table_ref->get_alias();

    // Test TblColRef
    auto find_col_ref = root_fact_table_ref->get_column(col2);
    LOG(INFO) << "Column Identity: " << find_col_ref->get_identity();

    // Test TableDesc
    auto& root_fact_table_desc = root_fact_table_ref->get_table_desc();
    const std::vector<ColumnDesc>& columns = root_fact_table_desc.get_columns();

    // Test ColumnnDesc
    LOG(INFO) << "The columns in the fact table: ";
    for (std::vector<ColumnDesc>::const_iterator it = columns.begin(); it != columns.end(); it++)
        LOG(INFO) << it->get_name() << " " << it->get_data_type();

    /*End of Test DataModel*/

    return 0;
}
