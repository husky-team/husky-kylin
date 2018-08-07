#include "TableDesc.hpp"
#include "ColumnDesc.hpp"

#include <fstream>

TableDesc::TableDesc() {
	std::string tableJsonPath = "table.json"; // should be in hdfs
	std::ifstream ifs(tableJsonPath); 
	json j = json::parse(ifs);

	// for now, only support a fact table
	name = j["name"].get<std::string>();
	json j_columns = j["columns"];
	for(json::iterator it = j_columns.begin(); it != j_columns.end(); it++){
		std::string col_id = (*it)["id"].get<std::string>();
		std::string col_name = (*it)["name"].get<std::string>();
		std::string col_datatype = (*it)["datatype"].get<std::string>();

		columns.push_back(new ColumnDesc(col_id, col_name, col_datatype));
	}
}

TableDesc::~TableDesc() {}

std::vector<ColumnDesc *> TableDesc::getColumns() {
	return columns;
}

std::string TableDesc::getName() {
	return name;
}