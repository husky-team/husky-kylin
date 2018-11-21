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

#include "core-cube/cuboid/tree_cuboid_scheduler.hpp"

#include <cinttypes>

#include <set>
#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"

namespace husky {
namespace cube {
namespace {

class TestTreeCuboidScheduler : public testing::Test {
   public:
    std::vector<uint64_t> ALL_CUBOID_IDS = {
        31L,                          // 11111
        30L, 27L, 15L,                // 11110, 11011, 01111
        28L, 26L, 22L, 14L, 11L,      // 11100, 11010, 10110, 01110, 01011
        20L, 18L, 17L, 6L,  5L,  3L,  // 10100, 10010, 10001, 00110, 00101, 00011
        16L, 8L,  4L,  2L,  1L,       // 10000, 01000, 00100, 00010, 00001
        0L                            // 00000
    };

    std::unordered_map<uint64_t, std::set<uint64_t>> parent_child = {
        {31L, {30L, 27L, 15L}},  // 11111 | 11110 11011 01111
        {30L, {28L, 22L}},       // 11110 | 11100 10110
        {28L, {}},               // 11100 |
        {27L, {26L, 17L}},       // 11011 | 11010 10001
        {26L, {}},               // 11010 |
        {22L, {20L, 18L}},       // 10110 | 10100 10010
        {20L, {}},               // 10100 |
        {18L, {}},               // 10010 |
        {17L, {16L}},            // 10001 | 10000
        {16L, {}},               // 10000 |
        {15L, {14L, 11L, 5L}},   // 01111 | 01110 01011 00101
        {14L, {6L}},             // 01110 | 00110
        {11L, {8L, 3L}},         // 01011 | 01000 00011
        {8L, {}},                // 01000 |
        {6L, {}},                // 00110 |
        {5L, {4L}},              // 00101 | 00100
        {4L, {}},                // 00100 |
        {3L, {2L, 1L}},          // 00011 | 00010 00001
        {2L, {}},                // 00010 |
        {1L, {0L}},              // 00001 | 00000
        {0L, {}}                 // 00000 |
    };
};

TEST_F(TestTreeCuboidScheduler, Constructor) {
    auto scheduler = new TreeCuboidScheduler(nullptr, ALL_CUBOID_IDS);
    EXPECT_TRUE(nullptr != scheduler);
    delete scheduler;
}

TEST_F(TestTreeCuboidScheduler, GetAllCuboidIds) {
    TreeCuboidScheduler scheduler(nullptr, ALL_CUBOID_IDS);
    auto cuboid_id_set = scheduler.get_all_cuboid_ids();
    std::set<uint64_t> check_set(ALL_CUBOID_IDS.begin(), ALL_CUBOID_IDS.end());

    EXPECT_EQ(check_set, cuboid_id_set);
}

TEST_F(TestTreeCuboidScheduler, GetCuboidCount) {
    // TODO(tatiana): fake CubeDesc for testing
    // TreeCuboidScheduler scheduler(ALL_CUBOID_IDS);
    // EXPECT_EQ(scheduler.get_cuboid_count(), ALL_CUBOID_IDS.size());
}

TEST_F(TestTreeCuboidScheduler, IsValid) {
    TreeCuboidScheduler scheduler(nullptr, ALL_CUBOID_IDS);
    // Valid ids
    for (auto cuboid_id : ALL_CUBOID_IDS) {
        EXPECT_EQ(scheduler.is_valid(cuboid_id), true);
    }
    // Invalid ids
    EXPECT_EQ(scheduler.is_valid(7L), false);
    EXPECT_EQ(scheduler.is_valid(9L), false);
    EXPECT_EQ(scheduler.is_valid(19L), false);
    EXPECT_EQ(scheduler.is_valid(29L), false);
    EXPECT_EQ(scheduler.is_valid(32L), false);
}

TEST_F(TestTreeCuboidScheduler, GetSpanningCuboid) {
    TreeCuboidScheduler scheduler(nullptr, ALL_CUBOID_IDS);
    for (auto cuboid_id : ALL_CUBOID_IDS) {
        auto children = scheduler.get_spanning_cuboid(cuboid_id);
        for (auto child : children) {
            EXPECT_EQ(child & ~cuboid_id, 0);                    // check if derivable
            EXPECT_EQ(parent_child[cuboid_id].count(child), 1);  // check if is parent
        }
    }
}

TEST_F(TestTreeCuboidScheduler, FindBestMatch) {
    TreeCuboidScheduler scheduler(nullptr, ALL_CUBOID_IDS);
    EXPECT_EQ(scheduler.find_best_match_cuboid(4L), 4L);
    EXPECT_EQ(scheduler.find_best_match_cuboid(9L), 11L);
    EXPECT_EQ(scheduler.find_best_match_cuboid(13L), 15L);
    EXPECT_EQ(scheduler.find_best_match_cuboid(29L), 31L);
}

}  // namespace
}  // namespace cube
}  // namespace husky
