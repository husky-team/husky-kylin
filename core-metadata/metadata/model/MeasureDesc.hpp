#include <string>

#include "kylin/functionDesc.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class MeasureDesc {
   public:
    MeasureDesc(std::string name.FunctionDesc* function);
    ~MeasureDesc();
    std::string getName();
    void setName(std::string name);
    FunctionDesc* getFunction();
    void setFunction(FunctionDesc* function);

   private:
    std::string name;
    FunctionDesc* function;
};