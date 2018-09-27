#ifndef _FUNCTION_DESC_H_
#define _FUNCTION_DESC_H_

#include <string>

#include "ParameterDesc.hpp"

class FunctionDesc {
   public:
    FunctionDesc(std::string& expression, ParameterDesc* parameter, const std::string& returnType);
    ~FunctionDesc();

    const static std::string FUNC_SUM = "SUM";
    const static std::string FUNC_MIN = "MIN";
    const static std::string FUNC_MAX = "MAX";
    const static std::string FUNC_COUNT = "COUNT";
    const static std::set<std::string> BUILD_IN_AGGREGATIONS = {"SUM", "MIN", "MAX", "COUNT"};
    const static std::string PARAMETER_TYPE_CONSTANT = "constant";
    const static std::string PARAMETER_TYPE_COLUMN = "column";
    void init(DataModelDesc* model);

   private:
    // from json
    std::string expression;
    ParameterDesc* parameter;  // just one parameter
    std::string returnType;

    // computed attributes
    DataType* returnDataType;
    MeasureType* measureType;
};

#endif