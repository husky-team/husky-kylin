#include "TableRef.hpp"

#include "ColumnDesc.hpp"
#include "DataModelDesc.hpp"
#include "TableDesc.hpp"
#include "TblColRef.hpp"

TableRef::TableRef(DataModelDesc* model, const std::string& alias, TableDesc* table) {
    this->model = model;
    modelName = model->getName();
    this->alias = alias;
    this->table = table;
    std::vector<ColumnDesc*> column_vector = table->getColumns();
    for (std::vector<ColumnDesc*>::iterator it = column_vector.begin(); it != column_vector.end(); ++it) {
        std::string columnName = (*it)->getName();
        columns.insert(std::make_pair(columnName, new TblColRef(this, (*it))));
    }
}

TableRef::~TableRef() {}

TblColRef* TableRef::getColumn(const std::string& name) { return columns.find(name)->second; }

std::string TableRef::getAlias() { return alias; }

TableDesc* TableRef::getTableDesc() { return table; }

std::string TableRef::getTableName() { return table->getName(); }