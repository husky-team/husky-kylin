#include <sstream>
#include <string>
#include <vector>

#include "boost/tokenizer.hpp"

#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"
#include "lib/aggregator_factory.hpp"

#include "husky-plugins/io/input/orc_inputformat.hpp"

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
    infmt.set_input(husky::Context::get_param("input"));
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
