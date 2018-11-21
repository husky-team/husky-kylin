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
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "glog/logging.h"
#include "nlohmann/json.hpp"

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

using nlohmann::json;

class CubeDesc {
   public:
    explicit CubeDesc(const json& cube_desc_json);
    CubeDesc() {}
    ~CubeDesc() {}

    void init() {}  // Init cube desc

    /* Getters */
    inline const std::string& get_name() const { return name_; }
    inline const std::string& get_model_name() { return model_name_; }
    inline const std::shared_ptr<DataModelDesc> get_model() const { return model_; }
    inline std::shared_ptr<DataModelDesc> get_model() { return model_; }
    inline std::shared_ptr<RowKeyDesc> get_row_key() const { return row_key_; }
    inline int get_parent_forward() const { return parent_forward_; }
    // inline std::vector<std::shared_ptr<RowKeyColDesc>> get_row_key_columns() const { return row_key_columns_; } //
    inline const std::vector<std::unique_ptr<AggregationGroup>>& get_aggregation_groups() const {
        return aggregation_groups_;
    }
    const std::vector<AggregationGroup*> get_aggregation_group_ptrs() const;

    inline void set_cuboid_scheduler(CuboidSchedulerBase* cuboid_scheduler) { cuboid_scheduler_ = cuboid_scheduler; }
    inline CuboidSchedulerBase* get_initial_cuboid_scheduler() {
        CHECK_EQ(true, cuboid_scheduler_ != nullptr);
        return cuboid_scheduler_;
    }
    inline const std::vector<std::shared_ptr<MeasureDesc>>& get_measures() const { return measures_; }
    inline const std::vector<std::shared_ptr<DimensionDesc>>& get_dimensions() const { return dimensions_; }
    inline const std::vector<std::shared_ptr<TblColRef>>& get_dimension_columns() const { return dimension_columns_; }
    std::vector<std::shared_ptr<TblColRef>> get_flat_table_columns() const;

    /* Setters */
    inline void set_name(const std::string& name) { name_ = name; }
    inline void set_model_name(const std::string& model_name) { model_name_ = model_name; }
    inline void set_model(const std::shared_ptr<DataModelDesc>& model) { model_ = model; }
    inline void set_row_key(const std::shared_ptr<RowKeyDesc>& row_key) { row_key_ = row_key; }

    /* Initializers */
    void init(const json& model_json, const std::map<std::string, std::shared_ptr<json>>& table_jsons);

   protected:
    void init_dimension_columns();
    void init_dimension_col_ref(const std::shared_ptr<TblColRef>& col_ref);
    void init_measure_columns();

   private:
    // std::list<Dictionary* > * dictionaries;
    std::vector<std::unique_ptr<AggregationGroup>> aggregation_groups_;
    std::string name_;
    std::string model_name_;
    std::vector<std::shared_ptr<DimensionDesc>> dimensions_;
    std::vector<std::shared_ptr<MeasureDesc>> measures_;
    int parent_forward_ = 3;
    uint64_t partition_date_start_ = 0L;
    uint64_t partition_date_end_ = 3153600000000L;

    std::shared_ptr<DataModelDesc> model_;
    // std::vector<std::shared_ptr<RowKeyColDesc>> row_key_columns_;
    std::shared_ptr<RowKeyDesc> row_key_;

    std::vector<std::shared_ptr<TblColRef>> dimension_columns_;
    std::map<std::shared_ptr<TblColRef>, uint32_t> dimension_columns_map_;

    // Avoid circular ownership
    CuboidSchedulerBase* cuboid_scheduler_;  // not owned, owned by CubeInstance, this is one of my owners
    std::set<uint64_t>* cuboid_black_set_;
    std::list<std::set<std::string>>* mandatory_dimension_set_list_;
};

}  // namespace cube
}  // namespace husky
