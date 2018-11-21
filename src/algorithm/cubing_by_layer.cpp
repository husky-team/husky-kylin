
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

#include "algorithm/cubing_by_layer.hpp"

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "core/engine.hpp"
#include "io/hdfs_manager.hpp"

#include "algorithm/base_cuboid_encoder.hpp"
#include "algorithm/cuboid_combiner.hpp"
#include "algorithm/cuboid_kv.hpp"
#include "algorithm/nd_cuboid_builder.hpp"
#include "core-cube/cuboid/cuboid.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "table/table.hpp"
#include "utils/hash.hpp"

namespace husky {
namespace cube {

auto& CubingByLayer::build_base_cuboid(Table& table) {
    auto base_cuboid_id = Cuboid::get_base_cuboid_id(cube_desc_);
    auto base_cuboid = cuboid_cache_.at(base_cuboid_id);

    auto& base_cuboid_objlist = ObjListStore::create_objlist<CuboidKV>();
    auto& base_cuboid_ch = ChannelStore::create_push_combined_channel<std::vector<std::string>, CuboidStringCombiner>(
        base_cuboid_objlist, base_cuboid_objlist);

    BaseCuboidEncoder base_cuboid_encoder(base_cuboid, table.get_column_idx());
    for (auto& row : table) {
        base_cuboid_ch.push(base_cuboid_encoder.get_string_values(row), base_cuboid_encoder.get_key(row));
    }
    base_cuboid_ch.out();  // flush channel

    list_execute(base_cuboid_objlist, {&base_cuboid_ch}, {},
                 [&base_cuboid_ch](CuboidKV& record) { record.set_measure_strs(base_cuboid_ch.get(record)); });
    save_to_hdfs(base_cuboid_objlist, output_path_, 0);
    table.clear();
    return base_cuboid_objlist;
}

void CubingByLayer::build_nd_cuboid(ObjList<CuboidKV>& base_cuboid_objlist) {
    auto total_levels = cuboid_scheduler_->get_build_level();
    std::vector<ObjList<CuboidKV>*> cuboids_by_layer(total_levels + 1);
    cuboids_by_layer[0] = &base_cuboid_objlist;
    NDCuboidBuilder cuboid_builder(cube_desc_);

    for (int i = 1; i <= total_levels; ++i) {
        auto& cuboids_i = ObjListStore::create_objlist<CuboidKV>();
        cuboids_by_layer[i] = &cuboids_i;
        auto& cuboids_parent = *cuboids_by_layer[i - 1];
        auto& cuboid_ch_i = ChannelStore::create_push_combined_channel<std::vector<std::string>, CuboidStringCombiner>(
            cuboids_parent, cuboids_i);

        // flatmap
        list_execute(cuboids_parent, {}, {&cuboid_ch_i}, [&cuboid_builder, &cuboid_ch_i, this](CuboidKV& record) {
            auto cuboid_id = cuboid_builder.get_row_key_splitter()->parse_cuboid(record.id());
            auto children = cuboid_scheduler_->get_spanning_cuboid(cuboid_id);
            if (!children.empty()) {
                cuboid_builder.get_row_key_splitter()->split(std::make_shared<std::vector<unsigned char>>(record.id()));
                auto parent_cuboid = cuboid_cache_.at(cuboid_id);
                for (auto child_id : children) {
                    auto child_cuboid = cuboid_cache_.at(child_id);
                    cuboid_ch_i.push(record.get_measure_strs(), *cuboid_builder.build_key(parent_cuboid, child_cuboid));
                }
            }
        });

        // double sum_price = 0;
        // reduce
        list_execute(cuboids_i, {&cuboid_ch_i}, {}, [&cuboid_ch_i](CuboidKV& record) {
            record.set_measure_strs(cuboid_ch_i.get(record));
            // sum_price += std::stod(cuboid_ch_i.get(record)[1]);
        });
        // LOG(INFO) << "sum price " << sum_price;
        // save cuboids
        save_to_hdfs(cuboids_i, output_path_, i);
        // clear upper layer cuboid objlist
        ObjListStore::drop_objlist(cuboids_parent.get_id());
    }
}

void CubingByLayer::build_cube(Table& table) {
    // create cuboids
    auto base_cuboid_id = Cuboid::get_base_cuboid_id(cube_desc_);
    cuboid_cache_.insert({base_cuboid_id, Cuboid::find_for_mandatory(cube_desc_, base_cuboid_id)});
    auto ids = cuboid_scheduler_->get_all_cuboid_ids();
    for (auto id : ids) {
        cuboid_cache_.insert({id, Cuboid::find_for_mandatory(cube_desc_, id)});
    }

    // build cube
    CuboidStringCombiner::set_aggregators(std::make_shared<MeasureAggregators>(cube_desc_->get_measures()));
    auto& base_cuboid_objlist = build_base_cuboid(table);
    build_nd_cuboid(base_cuboid_objlist);
}

#ifdef WITH_HDFS
void CubingByLayer::save_to_hdfs(ObjList<CuboidKV>& records, const std::string& output_path, int level) {
    // get path and namenode info
    std::string protocol = "hdfs://";
    CHECK_EQ(output_path.substr(0, protocol.size()), protocol) << "output path must start with hdfs://";
    auto colon = output_path.find(":", protocol.size());
    CHECK_EQ(true, colon != output_path.npos) << "namenode host and port should be specified as hostname:port";
    auto host = output_path.substr(protocol.size(), colon - protocol.size());
    auto path_start = output_path.find("/", colon);
    auto port = output_path.substr(colon + 1, path_start - colon - 1);
    std::string cuboid_output_path = get_cuboid_output_path(output_path.substr(path_start), level);

    list_execute(records, {}, {}, [&host, &port, &cuboid_output_path](CuboidKV& record) {
        husky::io::HDFS::Write(host, port, (char*) record.id().data(), record.id().size(), cuboid_output_path,
                               Context::get_global_tid());
        std::stringstream ss;
        for (auto& measure : record.get_measure_strs()) {
            ss << "\0" << measure;
        }
        ss << "\n";
        husky::io::HDFS::Write(host, port, ss.str(), cuboid_output_path, Context::get_global_tid());
    });
    husky::io::HDFS::CloseFile(host, port);
}
#else
void CubingByLayer::save_to_hdfs(ObjList<CuboidKV>& records, const std::string& output_path, int level) {
    LOG(ERROR) << "HDFS is not supported";
}
#endif

std::string CubingByLayer::get_cuboid_output_path(const std::string& output_path, int level) {
    if (level == 0) {
        return output_path + "level_base_cuboid";
    } else {
        return output_path + "level_" + std::to_string(level) + "_cuboid";
    }
}

}  // namespace cube
}  // namespace husky
