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

namespace husky {
namespace cube {

class RowKeyConstants {
   public:
    static const int ROWKEY_COL_DEFAULT_LENGTH = 256;
    static const int ROWKEY_CUBOIDID_LENGTH = 8;  // uint64_t
    static const int ROWKEY_SHARDID_LEN = 2;      // row key shard length
    static const int ROWKEY_SHARD_AND_CUBOID_LEN = ROWKEY_CUBOIDID_LENGTH + ROWKEY_SHARDID_LEN;
    static const int ROWKEY_BUFFER_SIZE = 65 * 256;  // a little more than 64 dimensions * 256 bytes each
};

}  // namespace cube
}  // namespace husky
