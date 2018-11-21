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

#include "core/engine.hpp"

#include "algorithm/cubing_by_layer.hpp"
#include "core-cube/cube_instance.hpp"
#include "core-metadata/meta_loader.hpp"
#include "table/table.hpp"
#include "table/table_reader_factory.hpp"
#include "utils/print_string.hpp"

using husky::Context;
using husky::cube::CubeInstance;
using husky::cube::Cuboid;
using husky::cube::MetaLoader;
using husky::table::TableReaderFactory;
using husky::table::Table;
using husky::cube::CubingByLayer;

void execute() {
    // load metadata
    MetaLoader loader(Context::get_param("meta_url"));
    CubeInstance cube_instance("test_instance", loader.get_cube_desc(), false);
    auto& cube_desc = cube_instance.get_cube_desc();
    LOG(INFO) << "[Cube name] " << cube_desc->get_name();
    cube_desc->init(loader.get_model_desc(), loader.get_tables());
    cube_instance.init_cuboid_scheduler();

    // all cuboid ids
    auto ids = cube_instance.get_cuboid_scheduler()->get_all_cuboid_ids();
    LOG(INFO) << "Cuboid ids " << ids;

    // load flat table
    auto reader = TableReaderFactory::get_table_reader(Context::get_param("table_format"));
    reader->read_data(Context::get_param("hive_table"));
    Table table(reader->get_data(), cube_desc->get_flat_table_columns());

    // cubing
    CubingByLayer(cube_desc, cube_instance.get_cuboid_scheduler(), Context::get_param("output_path")).build_cube(table);
}

int main(int argc, char** argv) {
    google::LogToStderr();

    std::vector<std::string> args;
    args.push_back("hdfs_namenode");
    args.push_back("hdfs_namenode_port");
    args.push_back("hive_table");
    args.push_back("meta_url");
    args.push_back("table_format");
    args.push_back("output_path");

    if (husky::init_with_args(argc, argv, args)) {
        husky::run_job(execute);
        return 0;
    }
}
