#include <set>
#include <map>
#include <list>
#include <utility>
#include <math.h>

#include "kylin/cubeDesc.hpp"

class CuboidScheduler
{
public:
	CuboidScheduler(CubeDesc * cubeDesc);
	~CuboidScheduler();

private:
	long max;
	std::set<long> allCuboidIds;
	std::map<long, std::list<long>> parent2child;
};