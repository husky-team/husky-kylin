#ifndef _FUNCTION_DESC_H_
#define _FUNCTION_DESC_H_

#include <string>

#include "DataModelDesc.hpp"
#include "ParameterDesc.hpp"
#include "core-metadata/metadata/datatype/DataType.h"

class FunctionDesc {
   public:
    FunctionDesc(std::string& expression, ParameterDesc* parameter, const std::string& returnType);
    ~FunctionDesc();

    const static std::string FUNC_SUM;
    const static std::string FUNC_MIN;
    const static std::string FUNC_MAX;
    const static std::string FUNC_COUNT;

    const static std::set<std::string> BUILD_IN_AGGREGATIONS;
    const static std::string PARAMETER_TYPE_CONSTANT;
    const static std::string PARAMETER_TYPE_COLUMN;
    void init(DataModelDesc* model);
    bool isMin();
    bool isMax();
    bool isSum();
    bool isCount();
    std::string getExpression();
    void setExpression(const std::string& expression);
    ParameterDesc* getParameter();
    void setParameter(ParameterDesc* parameter);
    std::string getReturnType();
    void setReturnType(const std::string& returnType);
    DataType* getReturnDataType();

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
