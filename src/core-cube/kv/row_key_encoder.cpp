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

#include "core-cube/kv/row_key_encoder.hpp"
#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

RowKeyEncoder::RowKeyEncoder(const std::shared_ptr<CubeDesc>& cube_desc, std::shared_ptr<Cuboid> cuboid)
    : AbstractRowKeyEncoder(cube_desc, cuboid) {
    for (auto const& column : cuboid->get_columns()) {
        body_length_ += 4;  // hard code! Should be different by dimension type (int, data, string, ...)
    }
}

void RowKeyEncoder::encode(const std::vector<unsigned char>& body_bytes, std::vector<unsigned char>& output_buf) {
    // TODO(tatiana): what is the expected size of output_buf? better to have offset to avoid inserting at start
    output_buf = body_bytes;
    fill_header(output_buf);
}

std::vector<unsigned char> RowKeyEncoder::encode(std::map<std::shared_ptr<TblColRef>, std::string>& value_map) {
    const std::vector<std::shared_ptr<TblColRef>>& columns = cuboid_->get_columns();
    std::vector<std::string> values;
    for (auto const& column : columns) {
        values.push_back(value_map.find(column)->second);
    }
    return encode(values);
}

std::vector<unsigned char> RowKeyEncoder::encode(std::vector<std::string>& values) {
    std::vector<unsigned char> bytes;
    int offset = get_header_length();
    const std::vector<std::shared_ptr<TblColRef>>& columns = cuboid_->get_columns();

    for (int i = 0; i < columns.size(); i++) {
        std::shared_ptr<TblColRef> column = columns[i];
        // int colLength = colIO.getColumnLength(column); // hard code!
        int col_length = 4;
        fill_column_value(column, col_length, values[i], bytes, offset);
        offset += col_length;
    }

    fill_header(bytes);
    return bytes;
}

}  // namespace cube
}  // namespace husky
