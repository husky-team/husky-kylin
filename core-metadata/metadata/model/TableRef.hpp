#ifndef _TABLE_REF_H_
#define _TABLE_REF_H_

#include <map>
#include <string>
#include <utility>

// #include "DataModelDesc.hpp"
// #include "TableDesc.hpp"
// #include "TblColRef.hpp"
class DataModelDesc;
class TableDesc;
class TblColRef;

class TableRef
{
public:
	TableRef(DataModelDesc * model, const std::string& alias, TableDesc * table);
	~TableRef();
	TblColRef * getColumn(const std::string& name);
	std::string getAlias();
	TableDesc * getTableDesc();
	std::string getTableName();
	
private:
	DataModelDesc * model;
	std::string alias;
	TableDesc * table;
	std::map<std::string, TblColRef * > columns;
	std::string modelName;
};

#endif