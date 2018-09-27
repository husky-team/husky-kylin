#include "core-metadata/metadata/model/TableDesc.hpp"

#include "gtest/gtest.h"

namespace husky {
namespace {

class TestTableDesc : public testing::Test {
   public:
    TestTableDesc() {}
    ~TestTableDesc() {}

   protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestTableDesc, Constructor) {
    // auto tableDesc = new TableDesc();
    // EXPECT_TRUE(tableDesc != nullptr);
}

}  // namespace
}  // namespace husky
