#include "CubeDesc.hpp"

#include "core-cube/cuboid/Cuboid.hpp"
#include "core-cube/cuboid/CuboidScheduler.hpp"
#include "core-metadata/metadata/model/DataModelDesc.hpp"
#include "core-metadata/metadata/model/FunctionDesc.hpp"
#include "core-metadata/metadata/model/MeasureDesc.hpp"
#include "core-metadata/metadata/model/ParameterDesc.hpp"

CubeDesc::CubeDesc(const std::string& cubeDescJsonPath, ) {
    std::ifstream ifs(cubeDescJsonPath);
    json j = json::parse(ifs);

    name = j["name"].get<std::string>();
    modelName = j["model_name"].get<std::string>();
    if (!j["parent_forward"].is_null())
        parentForward = j["parent_forward"].get<int>();
    cuboidScheduler = CuboidScheduler(this);

    // init model
    DataModelDesc* init_model = new DataModelDesc();
    setModel(init_model);

    // init dimensions
    json j_dimensions = j["dimensions"];
    for (json::iterator it = j_dimensions.begin(); it != j_dimensions.end(); it++) {
        std::string d_name = (*it)["name"].get<std::string>();
        std::string d_column = (*it)["column"].get<std::string>();
        std::string d_table = (*it)["table"].get<std::string>();

        dimensions.push_back(new DimensionDesc(this, d_name, d_table, d_column));
    }

    // init measures
    json j_measures = j["measures"];
    ParameterDesc* parameter;
    FunctionDesc* function;
    for (json::iterator it = j_measures.begin(); it != j_measures.end(); it++) {
        std::string m_name = (*it)["name"].get<std::string>();

        json j_function = (*it)["function"];
        std::string f_expression = j_function["expression"].get<std::string>();
        std::string f_returType = j_function["returnType"].get<std::string>();

        json j_parameter = j_function["parameter"];
        std::string p_type = j_parameter["type"].get<std::string>();
        std::string p_value = j_parameter["value"].get<std::string>();

        parameter = new ParameterDesc(p_type, p_value);
        function = new FunctionDesc(f_expression, parameter, f_returnType);

        measures.push_back(new MeasureDesc(m_name, function));
    }
}

std::string CubeDesc::getName() { return name; }

void CubeDesc::setName(std::string name) { this->name = name; }

std::string CubeDesc::getModelName() { return modelName; }

void CubeDesc::setModelName(std::string modelName) { this->modelName = modelName; }

DataModelDesc* CubeDesc::getModel() { return model; }

void CubeDesc::setModel(DataModelDesc* model) { this->mdoel = model; }

void CubeDesc::init() {
    // init model
    DataModelDesc* init_model = new DataModelDesc();
    setModel(init_model);

    // init dimensions
    for (std::list<DimensionDesc*>::iterator it = dimensions.begin(); it != dimensions.end(); ++it) {
        *it->init(this);
    }

    initDimensionColumns();
    initMeasureColumns();

    // init rowKey
    rowKey.init(this);

    // init aggregation columns; implement later
    // 	for(std::list<AggregationGroup* >::iterator it = aggregationGroups.begin(); it != aggregationGroups.end(); ++it)
    // {
    // 		agg.init(this, rowKey);
    // 	}
}
