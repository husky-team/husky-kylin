#include <string>
#include <utility>
#include <vector>
#include <list>

#include "kylin/cubeSegment.hpp"
#include "kylin/cubeDesc.hpp"
#include "kylin/tblColRef.hpp"
#include "kylin/cuboidScheduler.hpp"
#include "kylin/utils.hpp"

#ifndef CUBOID_H
#define CUBOID_H

class Cuboid
{
public:
	Cuboid(CubeDesc cubeDesc, long originalID, long validID);
	~Cuboid();

private:
	CubeDesc cubeDesc;
	long inputID;
	long id;
	std::vector<unsigned char> idBytes;
	std::list<TblColRef* > dimensionColumns;
	std::list<TblColRef* > translatedIDToColumns(long cuboidID);
};

#endif