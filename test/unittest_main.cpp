#include "glog/logging.h"
#include "gtest/gtest.h"

GTEST_API_ int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::LogToStderr();
    return RUN_ALL_TESTS();
}
