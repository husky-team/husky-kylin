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

#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "boost/tokenizer.hpp"

#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"
#include "lib/aggregator_factory.hpp"

#include "husky-plugins/io/input/orc_inputformat.hpp"
#include "husky-plugins/nlohmann/json.hpp"


class Word {
   public:
    using KeyT = std::string;
    Word() = default;
    explicit Word(const KeyT& w) : word(w) {}
    const KeyT& id() const { return word; }
    KeyT word;
    int count = 0;
};

void wc() {
    husky::io::ORCInputFormat infmt;
    std::ifstream input_json(husky::Context::get_param("input"));
    using json = nlohmann::json;
    json j_obj;
    input_json >> j_obj;
    std::string url = j_obj["url"];
    infmt.set_input(url);

    auto& word_list = husky::ObjListStore::create_objlist<Word>();
    auto& ch = husky::ChannelStore::create_push_combined_channel<int, husky::SumCombiner<int>>(infmt, word_list);
    auto parse_wc = [&](boost::string_ref& chunk) {
        if (chunk.size() == 0)
            return;
        boost::char_separator<char> sep("{}");
        boost::tokenizer<boost::char_separator<char>> tok(chunk, sep);
        std::ostringstream os;
        for (auto& w : tok) {
            os << w;
            std::cout << w << std::endl;
        }
    };
    husky::load(infmt, parse_wc);
    
}

int main(int argc, char** argv) {
    std::vector<std::string> args;
    args.push_back("hdfs_namenode");
    args.push_back("hdfs_namenode_port");
    args.push_back("input");
    
    if (husky::init_with_args(argc, argv, args)) {
        husky::run_job(wc);
        return 0;
    }
    return 1;
}
