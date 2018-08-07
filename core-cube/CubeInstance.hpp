#include <string>

#include "kylin/cubeDesc.hpp"
#include "kylin/cubeScheduler.hpp"


class CubeInstance
{
public:
	CubeInstance(std::string cubeName, CubeDesc * cubeDesc);
	~CubeInstance();
	
private:
	std::string name;
	std::string descName;
	CubeDesc * cubeDesc;
	CubeScheduler * cubeScheduler;
};