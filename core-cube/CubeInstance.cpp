#include "kylin/cubeInstance.hpp"

CubeInstance::CubeInstance(std::string cubeName, CubeDesc* cubeDesc) {
    this->name = cubeName;
    this->cubeDesc = cubeDesc;
    this->descName = cubeDesc->getName();
    this->cubeScheduler = cubeDesc->getCuboidScheduler();
}