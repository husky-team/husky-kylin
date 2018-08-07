#include "TblColRef.hpp"
#include "TableRef.hpp"
#include "ColumnDesc.hpp"

TblColRef::TblColRef(ColumnDesc * column) {
	this->column = column;
}

TblColRef::TblColRef(TableRef * table, ColumnDesc * column) {
	this->table = table;
	this->column = column;
	this->identity = getTableAlias() + "." + getName();
}

TblColRef::~TblColRef() {}

ColumnDesc * TblColRef::getColumnDesc() {
	return column;
}

std::string TblColRef::getName() {
	return column->getName();
}

std::string TblColRef::getTableAlias() {
	return table->getAlias();
}

std::string TblColRef::getIdentity() {
	return identity;
}