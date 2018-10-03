#include "DataModelDesc.hpp"

#include "ModelDimensionDesc.hpp"
#include "TableDesc.hpp"
#include "TableRef.hpp"
#include "TblColRef.hpp"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <stdexcept>
#include <utility>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

DataModelDesc::DataModelDesc(const std::string& modelJsonPath, const std::string& tableJsonPath) {
    // std::string modelJsonPath = "model.json"; // should be in hdfs
    std::ifstream ifs(modelJsonPath);
    json j = json::parse(ifs);

    name = j["name"].get<std::string>();
    // owner = j["owner"].get<std::string>();
    rootFactTable = j["fact_table"].get<std::string>();

    // init rootFactTableRef
    TableDesc* rootFactTableDesc = new TableDesc(tableJsonPath);
    std::string rootFactTableName = rootFactTableDesc->getName();
    rootFactTableRef = new TableRef(this, rootFactTableName, rootFactTableDesc);
    addTableName(rootFactTableName, rootFactTableRef);

    // init dimensions
    json j_dimensions = j["dimensions"];
    for (json::iterator it = j_dimensions.begin(); it != j_dimensions.end(); it++) {
        std::string table = (*it)["table"].get<std::string>();

        json j_columns = (*it)["columns"];
        std::vector<std::string> columns;
        for (json::iterator it = j_columns.begin(); it != j_columns.end(); it++) {
            columns.push_back((*it).get<std::string>());
        }

        dimensions.push_back(new ModelDimensionDesc(table, columns));
    }

    // init metrics
    json j_metrics = j["metrics"];
    for (json::iterator it = j_metrics.begin(); it != j_metrics.end(); it++) {
        std::string metric_name = (*it).get<std::string>();
        metrics.push_back(findColumn(metric_name)->getIdentity());
    }
}

DataModelDesc::~DataModelDesc() {}

std::string DataModelDesc::getName() { return name; }

// std::string DataModelDesc::getOwner() {
// 	return owner;
// }

// void DataModelDesc::setOwner(const std::string& owner) {
// 	this->owner = owner;
// }

TableRef* DataModelDesc::getRootFactTableRef() { return rootFactTableRef; }

void DataModelDesc::addTableName(std::string& name, TableRef* ref) {
    if (tableNameMap.count(name)) {
        tableNameMap.insert(std::pair<std::string, TableRef*>(name, NULL));
    } else {
        tableNameMap.insert(std::pair<std::string, TableRef*>(name, ref));
        allTableRefs.insert(ref);
    }
}

// void DataModelDesc::addAlias(TableRef * ref) {
// 	std::string alias = ref->getAlias();
// 	if(aliasMap.find(alias) != alias.end())
// 		throw std::logic_error("Alias '" + alias + "' ref to multiple tables")
// 	aliasMap.insert(std::make_pair(alias, ref));

// 	TableDesc * table = ref->getTableDesc();
// 	addTableName(table->getName(), ref);
// 	addTableName(table->getIdentity(), ref);
// }

TableRef* DataModelDesc::findTable(std::string& table) {
    boost::to_upper(table);
    TableRef* result = tableNameMap.find(table)->second;
    // error detection: result == NULL
    return result;
}

TblColRef* DataModelDesc::findColumn(std::string& table, std::string& column) {
    TableRef* tableRef = findTable(table);
    boost::to_upper(column);
    TblColRef* result = tableRef->getColumn(column);
    // error detection: result == NULL
    return result;
}

TblColRef* DataModelDesc::findColumn(std::string& column) {
    TblColRef* result = NULL;
    // std::string input = column;

    boost::to_upper(column);
    int cut = column.rfind('.');
    if (cut == std::string::npos) {
        // table not specified, try each table
        for (std::set<TableRef*>::iterator it = allTableRefs.begin(); it != allTableRefs.end(); ++it) {
            result = (*it)->getColumn(column);
            if (result != NULL)
                break;
        }
    } else {
        // table specified
        std::string tableName = column.substr(0, cut);
        std::string columnName = column.substr(cut + 1);
        result = findColumn(tableName, columnName);
    }

    return result;
}

// void DataModelDesc::init(TableDesc * rootTableDesc) {
// 	initRootTable(rootTableDesc);
// 	initDimensionAndMetrics();
// }

// void DataModelDesc::initRootTable(TableDesc * rootTableDesc) {
// 	rootFactTableRef = new TableRef(this, rootDesc.getName(), rootDesc);
// 	addTableName(rootTableDesc->getName(), rootFactTableRef);
// }

// void DataModelDesc::initDimensionAndMetrics();

// void DataModelDesc::initTableAlias(std::map<std::string, TableDesc * >& tables) {
// 	if(rootFactTable.empty())
// 		throw std::logic_error("root fact table should not be empty");

// 	boost::to_upper(rootFactTable);
// 	if (tables.find(rootFactTable) == tables.end())
// 		throw std::logic_error("Root fact table does not exist:" + rootFactTable);

// 	TableDesc * rootDesc = tables.find(rootFactTable);
// 	rootFactTableRef = new TableRef(this, rootDesc.getName(), rootDesc)
// 	addAlias(rootFactTableRef);
// }
