#ifndef _TABLE_DESC_H_
#define _TABLE_DESC_H_

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

// #include "ColumnDesc.hpp"
class ColumnDesc;

using json = nlohmann::json;

class TableDesc
{
public:
	TableDesc(const std::string & tableJsonPath);
	~TableDesc();
	// TabeleDesc * appendColumns(std::vector<ColumnDesc* > computedColumns, bool makeCopy);
	std::vector<ColumnDesc *> getColumns();
	std::string getName();

   private:
    std::string name;
    std::vector<ColumnDesc*> columns;
    // std::string tableType;
};

#endif
