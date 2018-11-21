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

#include "core-cube/kv/row_key_encoder.hpp"
#include "core-cube/kv/row_key_splitter.hpp"

namespace husky {
namespace cube {

class NDCuboidBuilder {
   public:
    explicit NDCuboidBuilder(const std::shared_ptr<CubeDesc>& cube_desc)
        : row_key_splitter_(std::make_unique<RowKeySplitter>(cube_desc)) {}
    std::shared_ptr<std::vector<unsigned char>> build_key(const std::shared_ptr<Cuboid>& parent,
                                                          const std::shared_ptr<Cuboid>& child) {
        auto row_key_encoder = get_rowkey_encoder(child);
        auto full_key_size = row_key_encoder->get_bytes_length();
        auto new_key_body_buf = std::make_shared<std::vector<unsigned char>>(full_key_size);
        build_key_internal(parent, child, new_key_body_buf);
        return new_key_body_buf;
    }

    const std::unique_ptr<RowKeySplitter>& get_row_key_splitter() const { return row_key_splitter_; }

   protected:
    void build_key_internal(const std::shared_ptr<Cuboid>& parent, const std::shared_ptr<Cuboid>& child,
                            const std::shared_ptr<std::vector<unsigned char>>& new_key_body_buf) {
        // rowkey columns
        auto parent_id = parent->get_id();
        uint64_t mask = utils::highest_one_bit(parent_id);
        uint64_t child_id = child->get_id();
        uint64_t parent_id_actual_length = 64L - utils::number_of_leading_zeros(parent_id);
        int index = row_key_splitter_->get_body_split_offset();  // skip shard and cuboidId
        // int offset = RowKeyConstants::ROWKEY_SHARD_AND_CUBOID_LEN;  // skip shard and cuboidId
        int offset = RowKeyConstants::ROWKEY_CUBOIDID_LENGTH;  // skip shard and cuboidId
        auto& split_buffers = row_key_splitter_->get_split_buffers();
        for (int i = 0; i < parent_id_actual_length; ++i) {
            if ((mask & parent_id) > 0) {     // if the this bit position equals 1
                if ((mask & child_id) > 0) {  // if the child cuboid has this column
                    CHECK_EQ(true, (offset + split_buffers[index].length()) <= new_key_body_buf->size())
                        << "offset " << offset << ", length " << split_buffers[index].length() << ", buf size "
                        << new_key_body_buf->size();
                    std::copy(split_buffers[index].begin(), split_buffers[index].end(),
                              new_key_body_buf->begin() + offset);
                    offset += split_buffers[index].length();
                }
                ++index;
            }
            mask = mask >> 1;
        }

        get_rowkey_encoder(child)->fill_header(*new_key_body_buf);
    }

    std::shared_ptr<RowKeyEncoder> get_rowkey_encoder(const std::shared_ptr<Cuboid>& cuboid) {
        auto cuboid_id = cuboid->get_id();
        auto pos = row_key_encoders_.find(cuboid_id);
        if (pos == row_key_encoders_.end()) {
            return row_key_encoders_.insert({cuboid_id, std::make_shared<RowKeyEncoder>(cuboid)}).first->second;
        }
        return pos->second;
    }

   private:
    std::map<uint64_t, std::shared_ptr<RowKeyEncoder>> row_key_encoders_;
    std::unique_ptr<RowKeySplitter> row_key_splitter_;
};

}  // namespace cube
}  // namespace husky
