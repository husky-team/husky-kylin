#include "DataType.hpp"

DataType::DataType(const std::string & name, int precision, int scale): name(name), precision(precision), scale(scale) {}

DataType::DataType(std::string dataType) {
	
}

DataType::~DataType() {}

DataType * DataType::getType(std::string type) {
	if(type.empty())
		return NULL;

	DataType * dataType = new DataType(type);
	return dataType;
}