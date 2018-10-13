#include "core-cube/cuboid/CuboidScheduler.hpp"

namespace husky {
namespace cube {

const std::vector<std::vector<long> > CuboidScheduler::get_cuboids_by_layer() {
	if (!cuboids_by_layer_.empty()) {
		return cuboids_by_layer_;
	}

	int total_um = 0;
	std::vector<long> base_cuboid_vec;
	base_cuboid_vec.push_back(get_base_cuboid_id(cube_desc_));
	cuboids_by_layer_.push_back(base_cuboid_vec);
	total_num++;

	std::vector<long> last_layer = base_cuboid_vec;
	while(!last_layer.empty()) {
		std::vector<long> new_layer;
		for(auto & parent : last_layer) {
			const std::vector<long> spanning_cuboid = get_spanning_cuboid(parent);
			new_layer.insert(new_layer.end(), spanning_cuboid.begin(), spanning_cuboid.end());
		}
		if(new_layer.empty()) { break; }
		cuboids_by_layer_.push_back(new_layer);
		total_num += new_layer.size();
		last_layer = new_layer;
	}

	int size = get_all_cuboids_ids.size();
	if(total_num != size) { return NULL; } // validate
	return cuboids_by_layer_;
}

}  // namespace cube
}  // namespace husky