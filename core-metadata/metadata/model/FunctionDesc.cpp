#include "FunctionDesc.hpp"

#include "../datatype/DataType.hpp"

#include <boost/algorithm/string.hpp>

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
        TblColRef* colRef = model.findColumn(p.getValue());
        // p.setValue(colRef.getIdentity());
        p.setColRef(colRef);
    }
}