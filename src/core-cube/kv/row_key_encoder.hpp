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

#include "core-cube/cuboid/cuboid.hpp"
#include "core-cube/kv/abstract_row_key_encoder.hpp"
#include "core-cube/kv/row_key_constants.hpp"
#include "core-metadata/dimension/dimension_encoding.hpp"
#include "core-metadata/dimension/integer_dim_enc.hpp"

#include <map>
#include <string>
#include <vector>

namespace husky {
namespace cube {

class RowKeyEncoder : public AbstractRowKeyEncoder {
   public:
    RowKeyEncoder(const std::shared_ptr<CubeDesc>& cube_desc, std::shared_ptr<Cuboid> cuboid);
    ~RowKeyEncoder() {}

    inline int get_header_length() const { return hearder_length_; }
    inline int get_bytes_length() const { return get_header_length() + body_length_; }
    inline void fill_header(std::vector<unsigned char>& bytes) {
        int offset = 0;  // for shard id(future use)
        std::vector<unsigned char> cuboid_id_bytes = cuboid_->get_bytes();
        bytes.insert(bytes.begin(), cuboid_id_bytes.begin(),
                     cuboid_id_bytes.begin() + RowKeyConstants::ROWKEY_CUBOIDID_LENGTH);
    }

    /* override */
    // inline std::vector<unsigned char> creat_buf() {
    // 	std::vector<unsigned char> v;
    // 	v.reserve(get_bytes_length());
    // 	return v;
    // }
    void encode(const std::vector<unsigned char>& body_bytes, std::vector<unsigned char>& output_buf) override;
    std::vector<unsigned char> encode(std::map<std::shared_ptr<TblColRef>, std::string>& value_map) override;
    std::vector<unsigned char> encode(std::vector<std::string>& values) override;

   private:
    int body_length_ = 0;
    // int uhc_off_set_ = -1; // it's a offset to the beginning of body (for shard id)
    // int uhc_length_ = -1;
    int hearder_length_;

    void fill_column_value(std::shared_ptr<TblColRef> column, int column_len, std::string& value_str,
                           std::vector<unsigned char>& output_value, int output_offset) {
        IntegerDimEnc integer_dim_enc(4);
        DimensionEncoding* dim_enc = &integer_dim_enc;  // hard code! Should get DimEnc by column.
        dim_enc->encode(value_str, output_value, output_offset);
    }
};

}  // namespace cube
}  // namespace husky
