#include "FunctionDesc.hpp"

#include "core-metadata/metadata/datatype/DataType.hpp"

#include <boost/algorithm/string.hpp>

const std::string FunctionDesc::FUNC_SUM = "SUM";
const std::string FunctionDesc::FUNC_MIN = "MIN";
const std::string FunctionDesc::FUNC_MAX = "MAX";
const std::string FunctionDesc::FUNC_COUNT = "COUNT";

const std::set<std::string> FunctionDesc::BUILD_IN_AGGREGATIONS{"SUM", "MIN", "MAX", "COUNT"};

const std::string FunctionDesc::PARAMETER_TYPE_CONSTANT = "constant";
const std::string FunctionDesc::PARAMETER_TYPE_COLUMN = "column";

FunctionDesc::FunctionDesc(std::string expression, ParameterDesc* parameter, std::string returnType) {
    boost::to_upper(expression);
    this->expression = expression;
    this->parameter = parameter;
    this->returnType = returnType;
    this->returnDataType = DataType::getType(returnType);
}

void FunctionDesc::init(DataModeldesc* model) {
    boost::to_upper(expression);
    returnDataType = DataType::getType(returnType);
    if (parameter.isColumnType()) {
        std::string = value = p.getValue();
        TblColRef* colRef = model.findColumn(value);
        // p.setValue(colRef.getIdentity());
        p.setColRef(colRef);
    }
}

bool FunctionDesc::isMin() { return expression.compare(FUNC_MIN) == 0; }

bool FunctionDesc::isMax() { return expression.compare(FUNC_MAX) == 0; }

bool FunctionDesc::isSum() { return expression.compare(FUNC_SUM) == 0; }

bool FunctionDesc::isCount() { return expression.compare(FUNC_COUNT) == 0; }

std::string FunctionDesc::getExpression() { return expression; }

void FunctionDesc::setExpression(const std::string& expression) { this->expression = expression; }
ParameterDesc* FunctionDesc::getParameter() { return parameter; }

void FunctionDesc::setParameter(ParameterDesc* parameter) { this->parameter = parameter; }

std::string FunctionDesc::getReturnType() { return returnType; }

void FunctionDesc::setReturnType(const std::string& returnType) {
    this->returnType = returnType;
    this->returnDataType = DataType::getType(returnType);
}

DataType* FunctionDesc::getReturnDataType() { return returnDataType; }
