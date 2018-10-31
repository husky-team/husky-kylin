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

#pragma once

#include <list>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "core-cube/cuboid/cuboid_scheduler_base.hpp"
#include "core-cube/cuboid/tree_cuboid_scheduler.hpp"
#include "core-cube/model/aggregation_group.hpp"
#include "core-cube/model/dimension_desc.hpp"
#include "core-cube/model/row_key_col_desc.hpp"
#include "core-cube/model/row_key_desc.hpp"
#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"
#include "core-metadata/metadata/model/parameter_desc.hpp"

namespace husky {
namespace cube {

class CubeDesc : public std::enable_shared_from_this<CubeDesc> {
   public:
    explicit CubeDesc(const std::string& cubeDescJsonPath);
    CubeDesc() {}
    ~CubeDesc() {}

    void init() {}  // Init cube desc

    /* Getters */
    inline const std::string& get_name() const { return name_; }
    inline const std::string& get_model_name() { return model_name_; }
    inline const std::shared_ptr<DataModelDesc> get_model() const { return model_; }
    inline std::shared_ptr<DataModelDesc> get_model() { return model_; }
    inline std::shared_ptr<RowKeyDesc> get_row_key() const { return row_key_; }
    // inline std::vector<std::shared_ptr<RowKeyColDesc>> get_row_key_columns() const { return row_key_columns_; } //
    // for test only
    inline std::list<AggregationGroup*> get_aggregation_groups() { return std::list<AggregationGroup*>(); }
    inline std::shared_ptr<CuboidSchedulerBase> get_initial_cuboid_scheduler() {
        if (cuboid_scheduler_ == nullptr)
            cuboid_scheduler_ = std::make_shared<TreeCuboidScheduler>(shared_from_this());
        return cuboid_scheduler_;
    }
    inline const std::vector<std::shared_ptr<MeasureDesc>> get_measures() const { return measures_; }
    inline const std::vector<std::shared_ptr<DimensionDesc>> get_dimensions() const { return dimensions_; }

    /* Setters */
    inline void set_name(const std::string& name) { name_ = name; }
    inline void set_model_name(const std::string& model_name) { model_name_ = model_name; }
    inline void set_model(const std::shared_ptr<DataModelDesc>& model) { model_ = model; }
    inline void set_row_key(const std::shared_ptr<RowKeyDesc>& row_key) { row_key_ = row_key; }

    /* Initializers */
    inline void init(const std::string& model_json_path, std::string& table_json_path) {
        /*For constuctors need shared_ptr(this)*/

        // init model
        model_ = std::make_shared<DataModelDesc>();
        model_->init(model_json_path, table_json_path);

        // init dimensions
        for (auto dimension : dimensions_) {
            dimension->init(shared_from_this());
        }

        // init rowkey
        row_key_->set_cube_desc(shared_from_this());
        // row_key_->set_row_key_columns(row_key_columns_);
    }

   private:
    // std::list<Dictionary* > * dictionaries;
    // std::list<AggregationGroup* > * aggregationGroups;
    std::string name_;
    std::string model_name_;
    std::shared_ptr<DataModelDesc> model_;
    // std::vector<std::shared_ptr<RowKeyColDesc>> row_key_columns_;
    std::shared_ptr<RowKeyDesc> row_key_;
    std::vector<std::shared_ptr<DimensionDesc>> dimensions_;
    std::vector<std::shared_ptr<MeasureDesc>> measures_;
    std::shared_ptr<CuboidSchedulerBase> cuboid_scheduler_;
    int parent_forward_ = 3;
    uint64_t partition_date_start_ = 0L;
    uint64_t partition_date_end_ = 3153600000000L;
    std::set<uint64_t>* cuboid_black_set_;
    std::list<std::set<std::string>>* mandatory_dimension_set_list_;
};

}  // namespace cube
}  // namespace husky
