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

#include "core-cube/model/row_key_col_desc.hpp"

#include <string>
#include <vector>

#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

RowKeyColDesc::RowKeyColDesc(const std::string& column, const std::string& encoding) {
    this->column_ = column;
    this->encoding_ = encoding;
}

void RowKeyColDesc::init(int index, const CubeDesc& cube_desc) {
    bit_index_ = index;
    col_ref_ = cube_desc.get_model().find_column(column_);
    column_ = col_ref_->get_identity();

    std::vector<std::string> encoding_params = DimensionEncoding::parse_encoding_conf(encoding_);
    if (encoding_params.size() == 1) {
        // no args
        encoding_name_ = encoding_params[0];
    } else {
        encoding_name_ = encoding_params[0];
        encoding_args_ = encoding_params[1];
    }
}

}  // namespace cube
}  // namespace husky
