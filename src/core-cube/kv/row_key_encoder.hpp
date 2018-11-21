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
#include <string>
#include <vector>

#include "core-cube/cuboid/cuboid.hpp"
#include "core-cube/kv/abstract_row_key_encoder.hpp"
#include "core-cube/kv/row_key_constants.hpp"
#include "core-cube/model/cube_desc.hpp"
#include "core-cube/model/row_key_desc.hpp"
#include "core-metadata/dimension/date_dim_enc.hpp"
#include "core-metadata/dimension/dimension_encoding.hpp"
#include "core-metadata/dimension/integer_dim_enc.hpp"

namespace husky {
namespace cube {

class DimensionEncodingFactory {
   public:
    // TODO(tatiana): support only date and int now
    static std::shared_ptr<DimensionEncoding> create(const std::string& encoding_name,
                                                     const std::vector<std::string>& encoding_args) {
        if (encoding_name == DateDimEnc::ENCODING_NAME) {
            return std::make_shared<DateDimEnc>(encoding_args);
        }
        if (encoding_name == IntegerDimEnc::ENCODING_NAME) {
            return std::make_shared<IntegerDimEnc>(std::stoi(encoding_args[0]));
        }
        return std::make_shared<IntegerDimEnc>(4);
        // TODO(tatiana): support dictionary
    }
};

class RowKeyEncoder : public AbstractRowKeyEncoder {
   public:
    explicit RowKeyEncoder(const std::shared_ptr<Cuboid>& cuboid);

    inline int get_header_length() const { return header_length_; }
    inline int get_bytes_length() const { return get_header_length() + body_length_; }

    void fill_header(std::vector<unsigned char>& bytes);

    inline std::vector<unsigned char> create_buf() override { return std::vector<unsigned char>(get_bytes_length()); }
    void encode(const std::vector<unsigned char>& body_bytes, std::vector<unsigned char>& output_buf) override;
    std::vector<unsigned char> encode(const std::map<std::shared_ptr<TblColRef>, std::string>& value_map) override;
    std::vector<unsigned char> encode(std::vector<std::string>& values) override;
    std::vector<unsigned char> encode(const std::vector<const std::string*>& values) override;

   protected:
    inline int get_column_length(const std::shared_ptr<TblColRef>& col) {
        return get_dim_enc(col)->get_length_of_encoding();
    }

    const std::shared_ptr<DimensionEncoding>& get_dim_enc(const std::shared_ptr<TblColRef>& col) {
        CHECK_EQ(true, dim_enc_map_ != nullptr);
        auto pos = dim_enc_map_->find(col);
        if (pos != dim_enc_map_->end()) {
            return pos->second;
        }
        auto col_desc = cuboid_->get_cube_desc()->get_row_key()->get_col_desc(col);
        auto ret = DimensionEncodingFactory::create(col_desc->get_encoding_name(), col_desc->get_encoding_args());
        return dim_enc_map_->insert({col, ret}).first->second;
    }

    inline void fill_column_value(const std::shared_ptr<TblColRef>& column, int column_len,
                                  const std::string& value_str, std::vector<unsigned char>& output_value,
                                  int output_offset) {
        get_dim_enc(column)->encode(value_str, output_value, output_offset);
    }

   private:
    int body_length_ = 0;
    // int uhc_off_set_ = -1; // it's a offset to the beginning of body (for shard id)
    // int uhc_length_ = -1;
    int header_length_ = RowKeyConstants::ROWKEY_CUBOIDID_LENGTH;  // TODO(tatiana): not supporting sharding now

    std::shared_ptr<std::map<std::shared_ptr<TblColRef>, std::shared_ptr<DimensionEncoding>>> dim_enc_map_;
};

}  // namespace cube
}  // namespace husky
