#include "kylin/cuboid.hpp"

Cuboid::Cuboid(CubeDesc cubeDesc, long originalID, long validID)
    : cubeDesc(cubeDesc), inputID(originalID), id(validID) {
    idBytes = longToBytes(this->id);
    dimensionColumns = translatedToColumns(this->id);
}

std::list<TblColRef*> Cuboid::translatedIDToColumns(long cuboidID) {}