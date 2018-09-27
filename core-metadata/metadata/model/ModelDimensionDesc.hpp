#ifndef _MDOEL_DIMENSION_DESC_H_
#define _MDOEL_DIMENSION_DESC_H_

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
// #include "DataModelDesc.cpp"
class DataModelDesc;

class ModelDimensionDesc {
   public:
    ModelDimensionDesc(std::string& table, const std::vector<std::string>& columns);
    ~ModelDimensionDesc();
    std::string getTable();
    void setTable(const std::string& table);
    std::vector<std::string> getColumns();
    void setColumns(const std::vector<std::string>& columns);
    // void init(DataModelDesc * model);

   private:
    std::string table;
    std::vector<std::string> columns;
};

#endif