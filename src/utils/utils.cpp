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
    CHECK_EQ(true, offset >= 0 && offset + size <= bytes.size()) << "bytes.size() = " << bytes.size()
                                                                 << ", offset = " << offset << ", size = " << size;
    for (int i = offset + size - 1; i >= offset; i--) {
        bytes[i] = (unsigned char) num;
        num >>= 8;
    }
}

uint64_t read_long(const std::vector<unsigned char>& bytes, int offset, int size) {
    CHECK_EQ(true, offset >= 0 && offset + size <= bytes.size()) << "bytes.size() = " << bytes.size()
                                                                 << ", offset = " << offset << ", size = " << size;
    uint64_t integer;
    for (int i = offset, n = offset + size; i < n; i++) {
        integer <<= 8;
        integer |= (uint64_t) bytes[i] & 0xFF;
    }
    return integer;
}

int bit_count(uint64_t n) {
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

uint64_t highest_one_bit(uint64_t n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n - (n >> 1);
}

uint64_t number_of_leading_zeros(uint64_t n) {
    if (n == 0L) {
        return 64;
    }
    int ret = 1;
    int half = (int) (n >> 32);
    if (half == 0) {
        ret += 32;
        half = (int) n;
    }

    if (half >> 16 == 0) {
        ret += 16;
        half <<= 16;
    }

    if (half >> 24 == 0) {
        ret += 8;
        half <<= 8;
    }

    if (half >> 28 == 0) {
        ret += 4;
        half <<= 4;
    }

    if (half >> 30 == 0) {
        ret += 2;
        half <<= 2;
    }

    ret -= half >> 31;
    return ret;
}

}  // namespace utils
}  // namespace husky
