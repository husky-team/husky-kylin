#ifndef _CUBE_DESC_H_
#define _CUBE_DESC_H_

#include <string>
#include <utility>
#include <vector>
#include <list>
#include <set>
#include <string>

#include "nlohmann/json.hpp"
#include "DataModelDesc.hpp"
#include "CuboidScheduler.hpp"

using json = nlohmann::json;

class CubeDesc
{
public:
	CubeDesc();
	~CubeDesc();
	std::string getName();
	void setName(std::string name);
	std::string getModelName();
	void setModelName(std::string modelName);
	DataModelDesc * getModel();
	void setModel(DataModelDesc * model);
	void init(); // init cubeDesc

private:
	std::string name;
	std:string modelName;
	std::list<DimensionDesc* > * dimensions;
	std::list<MeasureDesc* > * measures;
	// std::list<Dictionary* > * dictionaries;
	RowKeyDesc* rowKey;
	CuboidScheduler * cuboidScheduler;
	// std::list<AggregationGroup* > * aggregationGroups;
	long partitionDateStart = 0L;
	long partitionDateEnd = 3153600000000L;
	std::set<long> * cuboidBlackSet;
	int parentForward = 3;
	std::list<std::set<std::string>> * mandatoryDimensionSetList;
	DataModelDesc * model;
};

#endif