#ifndef _TBL_COL_REF_H_
#define _TBL_COL_REF_H_

#include <string>

// #include "TableRef.hpp"
// #include "ColumnDesc.hpp"
class TableRef;
class ColumnDesc;

class TblColRef
{
public:
	TblColRef(ColumnDesc * column);
	TblColRef(TableRef * table, ColumnDesc * column);
	~TblColRef();
	ColumnDesc * getColumnDesc();
	std::string getName();
	std::string getTableAlias();
	std::string getIdentity();
	
private:
	TableRef * table;
	ColumnDesc * column;
	std::string identity;
};

#endif