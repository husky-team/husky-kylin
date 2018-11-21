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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "boost/utility/string_ref.hpp"

#include "base/log.hpp"
#include "glog/logging.h"

#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace table {

using cube::TblColRef;

class Table {
   public:
    Table(std::vector<std::vector<std::string>>&& raw_data, std::vector<std::shared_ptr<TblColRef>>&& columns)
        : columns_(std::move(columns)), raw_data_(std::move(raw_data)) {
        row_num_ = raw_data_.size();
        if (row_num_ != 0) {
            column_num_ = raw_data_.front().size();
        }
    }

    const std::string& get_data(uint32_t row, uint32_t column) const {
        CHECK_EQ(true, validate(row, column));
        return raw_data_[row][column];
    }

    const std::map<std::shared_ptr<TblColRef>, int>& get_column_idx() {
        if (column_idx_.empty()) {
            int i = 0;
            for (auto& col : columns_) {
                column_idx_.insert({col, i});
                ++i;
            }
        }
        return column_idx_;
    }

    /** free memory */
    inline void clear() { std::vector<std::vector<std::string>>().swap(raw_data_); }
    inline auto begin() const { return raw_data_.begin(); }
    inline auto end() const { return raw_data_.end(); }
    inline auto begin() { return raw_data_.begin(); }
    inline auto end() { return raw_data_.end(); }

   protected:
    bool validate(uint32_t row, uint32_t column) const {
        if (row_num_ == 0) {
            husky::LOG_W << "false for row_num = 0";
            return false;
        } else if (row >= row_num_) {
            husky::LOG_I << "false for row >= row_num";
            return false;
        } else if (column >= column_num_) {
            husky::LOG_I << "false for column >= column_num";
            return false;
        }
        return true;
    }

   private:
    uint32_t row_num_ = 0;
    uint32_t column_num_ = 0;
    std::vector<std::shared_ptr<TblColRef>> columns_;
    std::vector<std::vector<std::string>> raw_data_;
    std::map<std::shared_ptr<TblColRef>, int> column_idx_;
};

}  //  namespace husky
}  //  namespace table
