#include "kylin/cuboidScheduler.hpp"

CuboidScheduler::CuboidScheduler(CubeDesc * cubeDesc) {
	this->cubeDesc = cubeDesc;
	int size = cubeDesc->getRowKey()->getRowKeyColumns()->length;
	this.max = (long)pow(2, size) - 1;

	std::pair<std::set<long>, std::map<long, std::list<long>>> pair = buildTreeBottompUp();
	this->allCuboidIDs = pair.getFirst();
	this->parent2child = pair.getSecond();
}