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

#include <string>
#include <vector>

#include "glog/logging.h"

#include "core-metadata/metadata/model/column_desc.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
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

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();

    // For testing only
    DataModelDesc* model = new DataModelDesc("./resources/model.json", "./resources/table.json");
    // LOG(INFO)<<"The model owner: "<<model->getOwner()<<std::endl;

    // Test DataModelDesc
    LOG(INFO) << "The model name: " << model->get_name();

    std::string table = "KYLIN_SALES";
    TableRef* find_table = model->find_table(table);
    LOG(INFO) << "Found table name: " << find_table->get_alias();

    std::string col1 = "PART_DT";
    TblColRef* find_tbl_col = model->find_column(table, col1);
    LOG(INFO) << "Found column by table: " << find_tbl_col->get_name();

    std::string col2 = "OPS_USER_ID";
    TblColRef* find_col = model->find_column(col2);
    LOG(INFO) << "Found column: " << find_col->get_name();

    // Test TableRef
    auto root_fact_table_ref = model->get_root_fact_table_ref();
    LOG(INFO) << "The root fact table name: " << root_fact_table_ref->get_table_name();
    LOG(INFO) << "The root fact table alias: " << root_fact_table_ref->get_alias();

    // Test TblColRef
    TblColRef* find_col_ref = root_fact_table_ref->get_column(col2);
    LOG(INFO) << "Column Identity: " << find_col_ref->get_identity();

    // Test TableDesc
    auto& root_fact_table_desc = root_fact_table_ref->get_table_desc();
    const std::vector<ColumnDesc>& columns = root_fact_table_desc.get_columns();

    // Test ColumnnDesc
    LOG(INFO) << "The columns in the fact table: ";
    for (std::vector<ColumnDesc>::const_iterator it = columns.begin(); it != columns.end(); it++)
        LOG(INFO) << it->get_name() << " " << it->get_data_type();

    return 0;
}
