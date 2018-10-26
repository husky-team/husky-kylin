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

#include <string>
#include <vector>
#include <memory>

#include "core-metadata/dimension/dimension_encoding.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

class CubeDesc;

class RowKeyColDesc {
   public:
    RowKeyColDesc(const std::string& column, const std::string& encoding);
    ~RowKeyColDesc() {}

    void init(int index, const std::shared_ptr<CubeDesc> & cube_desc);

    inline const std::string& get_encoding() const { return encoding_; }
    inline void set_encoding(const std::string& encoding) { this->encoding_ = encoding; }
    inline const std::string& get_column() const { return column_; }
    inline void set_column(const std::string& column) { this->column_ = column; }
    inline const std::string& get_index() const { return index_; }
    inline void set_index(const std::string& index) { this->index_ = index; }
    inline const std::string& get_encoding_name() const { return encoding_name_; }
    inline const std::string& get_encoding_args() const { return encoding_args_; }
    inline int get_bit_index() const { return bit_index_; }
    inline std::shared_ptr<TblColRef> get_col_ref() { return col_ref_; }

   private:
    std::string column_;
    std::string encoding_;
    std::string index_;

    // computed attributes (but only encoding integers for now)
    std::string encoding_name_;
    std::string encoding_args_;  // interger size
    // std::vector<std::string> encodingArgs;
    int bit_index_;
    std::shared_ptr<TblColRef> col_ref_;
};

}  // namespace cube
}  // namespace husky
