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

#include "gtest/gtest.h"

namespace husky {
namespace utils {
namespace {

class TestUtils : public testing::Test {};

TEST_F(TestUtils, IntToBytes) {
    auto bytes = int_to_bytes(0x12345678);
    ASSERT_EQ(bytes.size(), 4);
    EXPECT_EQ(bytes[0], 0x12);
    EXPECT_EQ(bytes[1], 0x34);
    EXPECT_EQ(bytes[2], 0x56);
    EXPECT_EQ(bytes[3], 0x78);
}

TEST_F(TestUtils, BytesToInt) {
    int num = bytes_to_int({0x87, 0x65, 0x43, 0x21});
    EXPECT_EQ(num, 0x87654321);
}

TEST_F(TestUtils, LongToBytes) {
    uint64_t num = 0x1232543243242345;
    auto bytes = long_to_bytes(num);
    ASSERT_EQ(bytes.size(), 8);
    EXPECT_EQ(bytes[0], 0x12);
    EXPECT_EQ(bytes[1], 0x32);
    EXPECT_EQ(bytes[2], 0x54);
    EXPECT_EQ(bytes[3], 0x32);
    EXPECT_EQ(bytes[4], 0x43);
    EXPECT_EQ(bytes[5], 0x24);
    EXPECT_EQ(bytes[6], 0x23);
    EXPECT_EQ(bytes[7], 0x45);
}

TEST_F(TestUtils, BytesToLong) {
    uint64_t num = bytes_to_long({0x12, 0x32, 0x54, 0x32, 0x43, 0x24, 0x23, 0x45});
    EXPECT_EQ(num, 0x1232543243242345);
}

TEST_F(TestUtils, ByteLongConversion) {
    for (uint64_t i = 0; i < 1000; ++i) {
        EXPECT_EQ(bytes_to_long(long_to_bytes(i)), i);
    }
}

TEST_F(TestUtils, WriteLong) {
    std::vector<unsigned char> bytes(24);
    uint64_t num = 0x1232543243242345;
    write_long(num, bytes, 16, 8);
    EXPECT_EQ(bytes[16], 0x12);
    EXPECT_EQ(bytes[17], 0x32);
    EXPECT_EQ(bytes[18], 0x54);
    EXPECT_EQ(bytes[19], 0x32);
    EXPECT_EQ(bytes[20], 0x43);
    EXPECT_EQ(bytes[21], 0x24);
    EXPECT_EQ(bytes[22], 0x23);
    EXPECT_EQ(bytes[23], 0x45);
}

TEST_F(TestUtils, ReadLong) {
    std::vector<unsigned char> bytes(24);
    uint64_t num = 0x1232543243242345;
    write_long(num, bytes, 8, 8);
    EXPECT_EQ(read_long(bytes, 8, 8), num);
}

}  // namespace
}  // namespace utils
}  // namespace husky
