#ifndef _MEASURE_DESC_H
#define _MEASURE_DESC_H

#include <string>

#include "FunctionDesc.h"

class MeasureDesc {
   public:
    MeasureDesc(const std::string& name.FunctionDesc* function);
    ~MeasureDesc();
    std::string getName();
    void setName(const std::string& name);
    FunctionDesc* getFunction();
    void setFunction(FunctionDesc* function);

   private:
    std::string name;
    FunctionDesc* function;
};

#endif
