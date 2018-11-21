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

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "core-cube/cuboid/cuboid.hpp"
#include "core-cube/kv/row_key_encoder.hpp"
#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

class BaseCuboidEncoder {
   public:
    const std::string DELIMITER = ":";

    BaseCuboidEncoder(const std::shared_ptr<Cuboid>& base_cuboid,
                      const std::map<std::shared_ptr<TblColRef>, int>& flat_table_column_idx)
        : base_cuboid_(base_cuboid) {
        init(flat_table_column_idx);
    }

    std::vector<unsigned char> get_key(const std::vector<std::string>& row) {
        std::vector<const std::string*> column_values;
        column_values.reserve(row_key_column_idx_.size());
        for (auto idx : row_key_column_idx_) {
            column_values.push_back(&(row[idx]));
        }
        return row_key_encoder_->encode(column_values);
    }

    // return input for each measure
    std::vector<std::string> get_string_values(const std::vector<std::string>& row) {
        std::vector<std::string> measures(base_cuboid_->get_cube_desc()->get_measures().size());
        for (size_t i = 0; i < measures.size(); ++i) {
            measures[i] = get_string_value(i, row);
        }
        return measures;
    }

    // return input for each measure in byte array format
    std::vector<std::vector<unsigned char>> get_byte_values(const std::vector<std::string>& row) {
        std::vector<std::vector<unsigned char>> measures(base_cuboid_->get_cube_desc()->get_measures().size());
        for (size_t i = 0; i < measures.size(); ++i) {
            measures[i] = get_byte_value(i, row);
        }
        return measures;
    }

   protected:
    std::string get_string_value(size_t idx, const std::vector<std::string>& row) const {
        auto input_to_measure = get_input_to_measure(idx, row);
        std::stringstream ss;
        ss << input_to_measure.front();
        for (int i = 1; i < input_to_measure.size(); ++i) {
            ss << DELIMITER << input_to_measure[i];
        }
        return ss.str();
    }

    std::vector<unsigned char> get_byte_value(size_t idx, const std::vector<std::string>& row) const {
        auto input_to_measure = get_input_to_measure(idx, row);
        // TODO(tatiana)
        // auto& measure = base_cuboid_->get_cube_desc()->get_measures()[idx];
        // return aggrIngesters[idxOfMeasure].valueOf(inputToMeasure, measure, dictionaryMap);
        return std::vector<unsigned char>();
    }

    std::vector<std::string> get_input_to_measure(size_t idx, const std::vector<std::string>& row) const {
        auto& function = base_cuboid_->get_cube_desc()->get_measures()[idx]->get_function();
        auto parameter = function->get_parameter();
        auto& column_idx = measure_column_idx_[idx];

        std::vector<std::string> input_to_measure;
        int col_idx = 0;
        while (parameter != nullptr) {
            if (function->is_count()) {
                input_to_measure.push_back("1");
            } else if (parameter->is_column_type()) {
                input_to_measure.push_back(row[column_idx[col_idx++]]);
            } else {
                input_to_measure.push_back(parameter->get_value());
            }
            parameter = parameter->next_parameter().get();
        }
        return input_to_measure;
    }

    void init(const std::map<std::shared_ptr<TblColRef>, int>& flat_table_column_idx) {
        row_key_encoder_ = std::make_unique<RowKeyEncoder>(base_cuboid_);
        // init row key column indexes
        auto& columns = base_cuboid_->get_columns();
        row_key_column_idx_.reserve(columns.size());
        for (auto& col : columns) {
            auto pos = flat_table_column_idx.find(col);
            row_key_column_idx_.push_back(pos->second);
        }
        // init measure column indexes
        auto& measures = base_cuboid_->get_cube_desc()->get_measures();
        measure_column_idx_.resize(measures.size());
        int measure_idx = 0;
        for (auto& measure : measures) {
            auto& function = measure->get_function();
            auto& col_refs = function->get_parameter()->get_col_refs();
            if (!col_refs.empty()) {
                auto& column_idx = measure_column_idx_[measure_idx];
                column_idx.reserve(col_refs.size());
                for (auto& col : col_refs) {
                    column_idx.push_back(flat_table_column_idx.at(col));
                }
            }
            measure_idx += 1;
        }
    }

   private:
    std::shared_ptr<Cuboid> base_cuboid_;

    // after init
    std::unique_ptr<RowKeyEncoder> row_key_encoder_;
    std::vector<int> row_key_column_idx_;               // column indexes for row key
    std::vector<std::vector<int>> measure_column_idx_;  // column indexes for each measure
    // std::vector<MeasureEnc> measure_ingesters_;
};

}  // namespace cube
}  // namespace husky
