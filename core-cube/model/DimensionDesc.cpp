#include "DimensionDesc.hpp"

#include "DataModelDesc.hpp"

DimensionDesc::DimensionDesc(CubeDesc * cubeDesc, std::string name, std::string table, std::string column)
: name(name), table(table), column(column) {
	// init table
	DataModelDesc * model = cubeDesc.getModel();
	tableRef = model.findTable(table);

	// init column
	column = model.findColumn(table, column);
}

DimensionDesc::~DimensionDesc() {}
