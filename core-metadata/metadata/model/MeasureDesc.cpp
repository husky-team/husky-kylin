#include "MeasureDesc.hpp"

MeasureDesc::MeasureDesc(string name, FunctionDesc* function) {
    // string cubeDescJsonPath = "cube_desc.json"; // should be in hdfs
    // std::ifstream ifs(cubeDescJsonPath);
    // json j = json::parse(ifs);

    this->name = name;
    this->function = function;
}

std::string MeasureDesc::getName() { return name; }

void MeasureDesc::setName(std::string name) { this->name = name; }

FunctionDesc* MeasureDesc::getFunction() { return function; }

void MeasureDesc::setFunction(FunctionDesc* function) { this->function = function; }