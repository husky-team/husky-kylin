#include <vector>

#include "nlohmann/json.hpp"

#include "base/log.hpp"
#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"

using JSON = nlohmann::json;

void execute() {
    // set chunk size to max
    auto& infmt =
        husky::io::InputFormatStore::create_chunk_inputformat(~0u >> 1);
    infmt.set_input(husky::Context::get_param("input"));
    auto parse_json = [&](boost::string_ref& chunk) {
        husky::LOG_I << JSON::parse(chunk).dump(1, ' ', true);
    };
    husky::load(infmt, parse_json);
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
