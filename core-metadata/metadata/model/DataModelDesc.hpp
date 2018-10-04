#ifndef _DATA_MODEL_DESC_H_
#define _DATA_MODEL_DESC_H_

#include <list>
#include <map>
#include <set>
#include <string>
#include <string>
#include <vector>
// #include "TableDesc.hpp"
// #include "TableRef.hpp"
// #include "TblColRef.hpp"
// #include "ModelDimensionDesc.hpp"
class TableDesc;
class TableRef;
class TblColRef;
class ModelDimensionDesc;

class DataModelDesc {
   public:
    DataModelDesc(const std::string& modelJsonPath, const std::string& tableJsonPath);
    ~DataModelDesc();
    std::string getName();
    // std::string getOwner();
    // void setOwner(const std::string& owner);
    TableRef* getRootFactTableRef();
    TableRef* findTable(std::string& table);
    TblColRef* findColumn(std::string& table, std::string& column);
    TblColRef* findColumn(std::string& column);

   private:
    /*For now, suppose only one fact table, no lookup tables*/

    // from json
    std::string name;
    // std::string owner;
    std::string rootFactTable;  // "fact_table"
    // std::vector<JoinTableDesc *> jointTables; //"lookups"
    std::list<ModelDimensionDesc*> dimensions;  // "dimensions"
    std::vector<std::string> metrics;           // "metrics"

    // computed attributes
    TableRef* rootFactTableRef;
    // std::set<TableRef *> factTableRefs;
    // std::set<TableRef *> loopkupTableRefs;
    std::set<TableRef*> allTableRefs;
    // std::map<std::string, TableRef* > aliasMap; // alias => TableRef, a table has exactly one alias
    std::map<std::string, TableRef*> tableNameMap;  // name => TableRef, a table maybe referenced by multiple names
    // JoinsTree * joinsTree;

    void addTableName(std::string& name, TableRef* ref);
    // void addAlias(TableRef * ref);
    // void initRootTable(TableDesc * rootTableDesc);
    // void initDimensionAndMetrics();
    // void init(TableDesc * rootTableDesc);
};

#endif
