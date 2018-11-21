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

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "glog/logging.h"

#include "core-metadata/metadata/model/tbl_col_ref.hpp"

namespace husky {
namespace cube {

RowKeyEncoder::RowKeyEncoder(const std::shared_ptr<Cuboid>& cuboid)
    : AbstractRowKeyEncoder(cuboid),
      dim_enc_map_(std::make_shared<std::map<std::shared_ptr<TblColRef>, std::shared_ptr<DimensionEncoding>>>()) {
    for (auto const& column : cuboid->get_columns()) {
        body_length_ += get_column_length(column);
    }
}

void RowKeyEncoder::encode(const std::vector<unsigned char>& body_bytes, std::vector<unsigned char>& output_buf) {
    CHECK_EQ(body_bytes.size(), (size_t) body_length_);
    CHECK_EQ(output_buf.size(), (size_t) get_bytes_length()) << "body_bytes " << body_bytes.size() << " output_buf "
                                                             << output_buf.size() << " header " << get_header_length();
    std::copy(body_bytes.begin(), body_bytes.end(), output_buf.begin() + get_header_length());
    fill_header(output_buf);
}

std::vector<unsigned char> RowKeyEncoder::encode(const std::map<std::shared_ptr<TblColRef>, std::string>& value_map) {
    const std::vector<std::shared_ptr<TblColRef>>& columns = cuboid_->get_columns();
    std::vector<std::string> values;
    values.reserve(columns.size());
    for (auto const& column : columns) {
        values.push_back(value_map.find(column)->second);
    }
    return encode(values);
}

std::vector<unsigned char> RowKeyEncoder::encode(const std::vector<const std::string*>& values) {
    auto bytes = create_buf();
    int offset = get_header_length();
    const std::vector<std::shared_ptr<TblColRef>>& columns = cuboid_->get_columns();

    for (int i = 0; i < columns.size(); i++) {
        std::shared_ptr<TblColRef> column = columns[i];
        int col_length = get_column_length(column);
        fill_column_value(column, col_length, *values[i], bytes, offset);
        offset += col_length;
    }

    fill_header(bytes);
    return bytes;
}

std::vector<unsigned char> RowKeyEncoder::encode(std::vector<std::string>& values) {
    auto bytes = create_buf();
    int offset = get_header_length();
    const std::vector<std::shared_ptr<TblColRef>>& columns = cuboid_->get_columns();

    for (int i = 0; i < columns.size(); i++) {
        std::shared_ptr<TblColRef> column = columns[i];
        int col_length = get_column_length(column);
        fill_column_value(column, col_length, values[i], bytes, offset);
        offset += col_length;
    }

    fill_header(bytes);
    return bytes;
}

void RowKeyEncoder::fill_header(std::vector<unsigned char>& bytes) {
    int offset = 0;  // for shard id(future use)
    auto& cuboid_id_bytes = cuboid_->get_bytes();
    std::copy(cuboid_id_bytes.begin(), cuboid_id_bytes.begin() + RowKeyConstants::ROWKEY_CUBOIDID_LENGTH,
              bytes.begin() + offset);
}

}  // namespace cube
}  // namespace husky
