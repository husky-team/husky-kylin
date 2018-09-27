#include <iostream>

#include "ColumnDesc.hpp"
#include "DataModelDesc.hpp"
#include "ModelDimensionDesc.hpp"
#include "TableDesc.hpp"
#include "TableRef.hpp"
#include "TblColRef.hpp"

int main() {
    // Fof testing only
    DataModelDesc* model = new DataModelDesc();
    // std::cout<<"The model owner: "<<model->getOwner()<<std::endl;
    std::cout << "The model name: " << model->getName() << std::endl;
    TableRef* rootFactTableRef = model->getRootFactTableRef();
    std::cout << "The table name: " << rootFactTableRef->getTableName() << std::endl;
    TableDesc* rootFactTableDesc = rootFactTableRef->getTableDesc();
    std::vector<ColumnDesc*> columns = rootFactTableDesc->getColumns();
    std::cout << "The columns in the fact table: " << std::endl;
    for (std::vector<ColumnDesc*>::iterator it = columns.begin(); it != columns.end(); it++)
        std::cout << (*it)->getName() << std::endl;
    return 0;
}