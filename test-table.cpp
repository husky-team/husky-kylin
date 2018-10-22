#include "core-cube/string_encoder.hpp"
#include "core-cube/row_key.hpp"
#include "core-cube/table.hpp"

#include <iostream>
#include <string>
#include "stdio.h"
#include <vector>

#include "base/log.hpp"
#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"
#include "lib/aggregator_factory.hpp"

#include "boost/tokenizer.hpp"


void execute() {
    Table t = Table();
    t.read_data();
    t.set_delimiter(char(1));
    t.cut_and_encode();
    std::string s1;
    std::string s2;
    std::string s3;
    std::string s4;
    t.get_data(0, 0, &s1);
    t.get_data(7, 7, &s2);
    t.get_data(6, 6, &s3);
    t.get_data(16, 16, &s4);
    husky::LOG_I << s1;
    husky::LOG_I << s2;
    husky::LOG_I << s3;
    husky::LOG_I << s4;
}

int main(int argc, char** argv) {
    std::vector<std::string> args;
    args.push_back("hdfs_namenode");
    args.push_back("hdfs_namenode_port");
    args.push_back("input");

    if (husky::init_with_args(argc, argv, args)) {
        husky::run_job(execute);
        return 0;
    }

    return 1;
}
