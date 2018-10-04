#include <list>
#include <set>
#include <string>

#include "core-cube/cuboid/cuboidScheduler.hpp"
#include "core-cube/model/CubeDesc.hpp"

class CubeInstance {
   public:
    CubeInstance(const std::string& cubeName, CubeDesc* cubeDesc);
    ~CubeInstance();
    CuboidScheduler* getCuboidScheduler();
    CubeDesc* getCubeDesc();
    std::string getRootFactTable();
    std::string getName();
    void setName(const std::string& name);
    std::list<MeasureDesc*> getMeasures();
    std::string getDescName();
    void setDescName(const std::string& descName);
    std::set<long> getCuboids();
    int getBuildLevel();

   private:
    std::string name;  // instance name
    std::string descName;
    CubeDesc* cubeDesc;
    CubeScheduler* cubeScheduler;
};
