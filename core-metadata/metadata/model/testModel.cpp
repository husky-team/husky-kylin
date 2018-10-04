#include <iostream>

#include "ColumnDesc.hpp"
#include "DataModelDesc.hpp"
#include "ModelDimensionDesc.hpp"
#include "TableDesc.hpp"
#include "TableRef.hpp"
#include "TblColRef.hpp"

int main(int argc, char** argv) {
    // Fof testing only
    DataModelDesc* model =
        new DataModelDesc("../core-metadata/metadata/model/model.json", "../core-metadata/metadata/model/table.json");
    // std::cout<<"The model owner: "<<model->getOwner()<<std::endl;

    // Test DataModelDesc
    std::cout << "The model name: " << model->getName() << std::endl;

    std::string table = "KYLIN_SALES";
    TableRef* find_table = model->findTable(table);
    std::cout << "Found table name: " << find_table->getAlias() << std::endl;

    std::string col1 = "PART_DT";
    TblColRef* find_tbl_col = model->findColumn(table, col1);
    std::cout << "Found column by table: " << find_tbl_col->getName() << std::endl;

    std::string col2 = "OPS_USER_ID";
    TblColRef* find_col = model->findColumn(col2);
    std::cout << "Found column: " << find_col->getName() << std::endl;

    // Test TableRef
    TableRef* rootFactTableRef = model->getRootFactTableRef();
    std::cout << "The root fact table name: " << rootFactTableRef->getTableName() << std::endl;
    std::cout << "The root fact table alias: " << rootFactTableRef->getAlias() << std::endl;

    // Test TblColRef
    TblColRef* find_col_ref = rootFactTableRef->getColumn(col2);
    std::cout << "Column Identity: " << find_col_ref->getIdentity() << std::endl;

    // Test TableDesc
    TableDesc* rootFactTableDesc = rootFactTableRef->getTableDesc();
    std::vector<ColumnDesc*> columns = rootFactTableDesc->getColumns();

    // Test ColumnnDesc
    std::cout << "The columns in the fact table: " << std::endl;
    for (std::vector<ColumnDesc*>::iterator it = columns.begin(); it != columns.end(); it++)
        std::cout << (*it)->getName() << " " << (*it)->getDatatype() << std::endl;

    return 0;
}
