#include "core-cube/cuboid/cuboid_scheduler_base.hpp"

#include "core-cube/model/cube_desc.hpp"

namespace husky {
namespace cube {

std::vector<std::vector<uint64_t> > CuboidSchedulerBase::get_cuboids_by_layer() {
	if (!cuboids_by_layer_.empty()) {
		return cuboids_by_layer_;
	}

	int total_num = 0;
	std::vector<uint64_t> base_cuboid_vec;
	base_cuboid_vec.push_back(get_base_cuboid_id());
	cuboids_by_layer_.push_back(base_cuboid_vec);
	total_num++;

	std::vector<uint64_t> last_layer = base_cuboid_vec;
	while(!last_layer.empty()) {
		std::vector<uint64_t> new_layer;
		for(auto & parent : last_layer) {
			const std::vector<uint64_t> spanning_cuboid = get_spanning_cuboid(parent);
			new_layer.insert(new_layer.end(), spanning_cuboid.begin(), spanning_cuboid.end());
		}
		if(new_layer.empty()) { break; }
		cuboids_by_layer_.push_back(new_layer);
		total_num += new_layer.size();
		last_layer = new_layer;
	}

	int size = get_all_cuboid_ids().size();
	// if(total_num != size) { return NULL; } // validate
	return cuboids_by_layer_;
}

}  // namespace cube
}  // namespace husky