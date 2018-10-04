#include <string>
#include <vector>

#include <core-cube/model/CubeDesc.hpp>
#include <core-cube/model/RowKeyDesc.hpp>
#include <core-cube/model/SelectRule.hpp>

class AggregationGroup {
   public:
    AggregationGroup(CubeDesc* cubeDesc, RowKeyDesc* rowKeyDesc);
    ~AggregationGroup();

   private:
    std::vector<std::string> includes;  // all dims in aggregation groups
    // SelectRule * selectRule;
    CubeDesc* cubeDesc;
};