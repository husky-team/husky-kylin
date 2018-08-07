#include "ModelDimensionDesc.hpp"
#include "DataModelDesc.hpp"

ModelDimensionDesc::ModelDimensionDesc(std::string & table, const std::vector<std::string>& columns){
	this->table = table;
	boost::to_upper(table);
	this->columns = columns;
}

ModelDimensionDesc::~ModelDimensionDesc() {}

// void ModelDimensionDesc::init(DataModelDesc * model) {
// 	boost::toupper(table);

// }

std::string ModelDimensionDesc::getTable() {
	return table;
}

void ModelDimensionDesc::setTable(const std::string& table) {
	this->table = table;
}

std::vector<std::string> ModelDimensionDesc::getColumns() {
	return columns;
}

void ModelDimensionDesc::setColumns(const std::vector<std::string>& columns) {
	this->columns = columns;
}