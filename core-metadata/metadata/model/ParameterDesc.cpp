#include "ParameterDesc.hpp"

ParameterDesc::ParameterDesc(const std::string & type, const std::string & value): type(type), value(value) {}

ParameterDesc::~ParameterDesc() {}

std::string ParameterDesc::getType() {
	return type;
}

void ParameterDesc::setType(std::string type) {
	this->type = type;
}

std::string ParameterDesc::getValue() {
	return value;
}

void ParameterDesc::setValue(std::string value) {
	this->value = value;
}

TblColRef * ParameterDesc::getColRef() {
	return colRef;
}

void ParameterDesc::setTblColRef(TblColRef * colRef) {
	this->colRef = colRef;
}

bool ParameterDesc::isColumnType() {
	return type.compare(FunctionDesc::PARAMETER_TYPE_COLUMN) == 0;
}