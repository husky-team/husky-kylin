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
#include <vector>

#include "core-cube/kv/row_key_constants.hpp"
#include "core-cube/kv/row_key_encoder.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

class ByteArray {
   public:
    ByteArray() {}
    ByteArray(const std::vector<unsigned char>& bytes, int offset, int length)
        : ByteArray(std::make_shared<std::vector<unsigned char>>(bytes), offset, length) {}
    ByteArray(const std::shared_ptr<std::vector<unsigned char>>& bytes, int offset, int length)
        : offset_(offset), length_(length), bytes_(bytes) {
        CHECK_EQ(offset < bytes_->size(), true) << "offset " << offset << ", size " << bytes_->size();
        CHECK_EQ(offset + length <= bytes_->size(), true) << "offset " << offset << ", length " << length << ", size "
                                                          << bytes_->size();
    }

    auto begin() const { return bytes_->begin() + offset_; }
    auto end() const { return begin() + length_; }
    size_t length() const { return length_; }

   private:
    std::shared_ptr<std::vector<unsigned char>> bytes_;
    size_t offset_ = 0;
    size_t length_ = 0;
};

class RowKeySplitter {
   public:
    explicit RowKeySplitter(const std::shared_ptr<CubeDesc>& cube_desc)
        : RowKeySplitter(cube_desc, cube_desc->get_row_key()->get_row_key_columns().size() + 2) {}
    RowKeySplitter(const std::shared_ptr<CubeDesc>& cube_desc, int split_len)
        : cube_desc_(cube_desc), split_buffers_(split_len), split_offsets_(split_len) {
        init_dim_enc_map();
    }

    uint64_t parse_cuboid(const std::vector<unsigned char>& bytes) {
        int offset = enable_sharding_ ? RowKeyConstants::ROWKEY_SHARDID_LEN : 0;
        return utils::read_long(bytes, offset, RowKeyConstants::ROWKEY_CUBOIDID_LENGTH);
    }

    int get_body_split_offset() {
        if (enable_sharding_) {
            return 2;  // shard + cuboid
        }
        return 1;  // cuboid
    }

    uint64_t split(const std::shared_ptr<std::vector<unsigned char>>& bytes) {
        buffer_size_ = 0;
        int offset = 0;

        // if (enable_sharding_) {
        //     // extract shard
        //     ByteArray shardSplit = new ByteArray(bytes, offset, RowConstants.ROWKEY_SHARDID_LEN);
        //     split_buffers_[this.bufferSize++] = shardSplit;
        //     offset += RowConstants.ROWKEY_SHARDID_LEN;
        //     // lastSplittedShard = Bytes.toShort(shardSplit.value, 0, shardSplit.length);
        //     shardId = Bytes.toShort(shardSplit.array(), shardSplit.offset());
        // }

        // extract cuboid id
        // split_buffers_[buffer_size_++] = ByteArray(bytes, offset, RowKeyConstants::ROWKEY_CUBOIDID_LENGTH);
        split_buffers_.at(buffer_size_++) = ByteArray(bytes, offset, RowKeyConstants::ROWKEY_CUBOIDID_LENGTH);
        offset += RowKeyConstants::ROWKEY_CUBOIDID_LENGTH;
        uint64_t last_splitted_cuboid_id = utils::read_long(*bytes, 0, RowKeyConstants::ROWKEY_CUBOIDID_LENGTH);
        auto cuboid = Cuboid::find_for_mandatory(cube_desc_, last_splitted_cuboid_id);

        // rowkey columns
        for (int i = 0; i < cuboid->get_columns().size(); ++i) {
            split_offsets_[i] = offset;
            auto col = cuboid->get_columns()[i];
            int col_length = get_column_length(col);
            split_buffers_[buffer_size_++] = ByteArray(bytes, offset, col_length);
            offset += col_length;
        }

        return last_splitted_cuboid_id;
    }

    inline size_t get_buffer_size() const { return buffer_size_; }
    inline const std::vector<ByteArray>& get_split_buffers() const { return split_buffers_; }

   protected:
    inline int get_column_length(const std::shared_ptr<TblColRef>& col) {
        return get_dim_enc(col)->get_length_of_encoding();
    }

    const std::shared_ptr<DimensionEncoding>& get_dim_enc(const std::shared_ptr<TblColRef>& col) {
        auto pos = dim_enc_map_.find(col);
        CHECK_EQ(pos != dim_enc_map_.end(), true) << col->get_identity() << " no dim enc";
        return pos->second;
    }

    void init_dim_enc_map() {
        for (auto& rowkey_col_desc : cube_desc_->get_row_key()->get_row_key_columns()) {
            auto col = rowkey_col_desc->get_col_ref();
            auto col_desc = cube_desc_->get_row_key()->get_col_desc(col);
            auto ret = DimensionEncodingFactory::create(col_desc->get_encoding_name(), col_desc->get_encoding_args());
            dim_enc_map_.insert({col, ret});
        }
    }

   private:
    bool enable_sharding_ = false;
    std::shared_ptr<CubeDesc> cube_desc_;

    std::vector<ByteArray> split_buffers_;
    std::vector<int> split_offsets_;
    size_t buffer_size_ = 0;

    std::map<std::shared_ptr<TblColRef>, std::shared_ptr<DimensionEncoding>> dim_enc_map_;
};

}  // namespace cube
}  // namespace husky
