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

#include "core-metadata/metadata/model/table_desc.hpp"

#include <fstream>

#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

namespace husky {
namespace {

using nlohmann::json;

class TestTableDesc : public testing::Test {
   public:
    TestTableDesc() {}
    ~TestTableDesc() {}

   protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestTableDesc, Constructor) {
    auto filename = "./resources/table.json";
    std::ifstream iss(filename);
    auto jtable = json::parse(iss);
    auto tableDesc = new cube::TableDesc(jtable);
    EXPECT_TRUE(tableDesc != nullptr);
}

}  // namespace
}  // namespace husky
