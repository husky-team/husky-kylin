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

#include "utils/utils.hpp"

#include <cinttypes>
#include <vector>

#include "glog/logging.h"

namespace husky {
namespace utils {

/** Long in java is 64-bit, so we use uint64_t here */

std::vector<unsigned char> int_to_bytes(int param_int) {
    std::vector<unsigned char> array_of_bytes(4);
    for (int i = 0; i < 4; i++) {
        array_of_bytes[3 - i] = (param_int >> (i * 8));
    }
    return array_of_bytes;
}

int bytes_to_int(const std::vector<unsigned char>& bytes) {
    return ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3]);
}

std::vector<unsigned char> long_to_bytes(uint64_t param_long) {
    std::vector<unsigned char> array_of_bytes(8);
    for (int i = 0; i < 8; ++i) {
        array_of_bytes[7 - i] = (int) ((param_long >> (i * 8)) & 0xFF);
    }
    return array_of_bytes;
}

uint64_t bytes_to_long(const std::vector<unsigned char>& bytes) {
    return ((uint64_t(bytes[0]) << 56) + (uint64_t(bytes[1]) << 48) + (uint64_t(bytes[2]) << 40) +
            (uint64_t(bytes[3]) << 32) + (bytes[4] << 24) + +(bytes[5] << 16) + (bytes[6] << 8) + bytes[7]);
}

void write_long(uint64_t num, std::vector<unsigned char>& bytes, int offset, int size) {
    CHECK(offset > 0 && offset + size <= bytes.size()) << "bytes.size() = " << bytes.size() << ", offset = " << offset
                                                       << ", size = " << size;
    for (int i = offset + size - 1; i >= offset; i--) {
        bytes[i] = (unsigned char) num;
        num >>= 8;
    }
}

uint64_t read_long(const std::vector<unsigned char>& bytes, int offset, int size) {
    CHECK(offset > 0 && offset + size <= bytes.size()) << "bytes.size() = " << bytes.size() << ", offset = " << offset
                                                       << ", size = " << size;
    uint64_t integer;
    for (int i = offset, n = offset + size; i < n; i++) {
        integer <<= 8;
        integer |= (uint64_t) bytes[i] & 0xFF;
    }
    return integer;
}

}  // namespace utils
}  // namespace husky
