#include <math.h>
#include <list>
#include <map>
#include <set>
#include <utility>

#include <list>
#include <map>
#include <set>
#include <string>

#include "core-cube/model/CubeDesc.hpp"

class CuboidScheduler {
   public:
    CuboidScheduler(CubeDesc* cubeDesc);
    ~CuboidScheduler();
    int getCuboidCount();
    std::set<long> getAllCuboidIds();
    std::list<long> getSpanningCuboid(long cuboid);
    bool isValid(long requestCuboid);
    long findBestMatchCuboid(long cuboid);
    long findBestMatchCuboid1(long cuboid);

   protected:
    std::pair<std::set<long>, std::map<long, std::list<long>>> buildTreeBottomUp();

   private:
    long max;
    std::set<long> allCuboidIds;
    std::map<long, std::list<long>> parent2child;
    long doFindBestMatchCuboid1(long cuboid);
    long getOnTreeParent(long child);
    long getParentOnPromise(long child, std::set<long> coll, int forward);
    std::set<long> getOnTreeParentsByLayer(std::set<long> children);
    std::set<long> getLowestCuboids();
    std::set<long> getOnTreeParents(long child, std::set<AggregationGroup*> groups);
};
