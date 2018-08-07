#include "ColumnDesc.hpp"

ColumnDesc::ColumnDesc(const std::string& id, const std::string& name, const std::string& datatype): id(id), name(name), datatype(datatype) {
	// string cubeDescJsonPath = "table.json"; // should be in hdfs
	// std::ifstream ifs(cubeDescJsonPath); 
	// json j = json::parse(ifs);

	// id = j["id"].get<std::string>();
	// name = j["name"].get<std::string>();
	// datatype = j["datatype"].get<std::string>();
}

ColumnDesc::~ColumnDesc() {}

std::string ColumnDesc::getId() {
	return id;
}

std::string ColumnDesc::getName() {
	return name;
}

std::string ColumnDesc::getDatatype() {
	return datatype;
}