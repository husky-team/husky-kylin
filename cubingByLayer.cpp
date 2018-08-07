#include <string>
#include <utility>
#include <vector>

#include "boost/tokenizer.hpp"

#include "base/log.hpp"
#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"
#include "lib/aggregator_factory.hpp"

#include "kylin/cubeDesc.hpp"
#include "kylin/cubeInstance.hpp"

namespace husky {

using lib::Aggregator;
using lib::AggregatorFactory;

class Row {
   public:
    using KeyT = std::string;

    Row() = default;
    explicit Row(const KeyT& k) : key(k) {}
    const KeyT& id() const { return key; }

    KeyT key;
    //int value = 0;
};

void execute(){
	CubeDesc * cubeDesc = new CubeDesc();
    CubeInstance * cubeInstance = new CubeInstance(Context::get_param("cubeName"), cubeDesc);

    int countMeasureIndex = 0;
    for()

	Aggregator<int> agg;
	auto& ac = AggregatorFactory::get_channel();

	// get input
    auto& infmt = husky::io::InputFormatStore::create_line_inputformat();
    infmt.set_input(husky::Context::get_param("inputPath"));

    auto& table = ObjListStore::create_objlist<Row>();
    auto& ch = ChannelStore::create_push_combined_channel<int, SumCombiner<int>>(infmt, table);

    // count measures needed to be aggregated; for simplest, only sum

    load(infmt, {&ch}, [&](boost::string_ref& chunk)) {
    	if(chunk.size() == 0)
    		return;
    	boost::char_separator<char> sep(" \t");
    	boost::tokenizer<boost::char_separator<char>> tok(chunk, sep);
    	// encode key and value pair
    	// eg: std::pair <char[], int> = std::make_pair(['1', '2'], 6)
    	//push to channel
    }

    // To synchronize using aggregator channel
    list_execute(table, {&ch}, {&ac}, [&](Word& word) {
        int o = ch.get(word);
        tot_word.update(1);
        tot_occurence.update(o);
        max_occurence.update({word.id(), o});
    });

    int totalLevels = cube.getCuboidScheduler().getBuildLevels();
    std::vector<std::vector<Cuboid* > > levelCuboids; // cube
    // for(int i = 0; i < totalLevels + 1; i++)
    // 	levelCuboids.push_back(std::vector<Cuboid*>(numOfColumns));
    int level = 0;

    // aggregate to calculate base cuboid
    // save to HDFS

    // aggregate to ND cuboids
}

} // namespace husky

int main(int argc, char** argv) {
	std::vector<std::string> args;
	// args.push_back("hdfs_namenode");
	// args.push_back("hdfs_namenode_port");
	args.push_back("metaUrl");
	args.push_back("inputPath");
	args.push_back("cubeName");
	args.push_back("segmentID");
	args.push_back("outputPath");

	if(husky::init_with_args(argc, argv, args)) {
		husky::run_job(husky::arggregator);
		return 0;
	}

	return 1;
}