#include "DataType.hpp"

const std::set<std::string> DataType::INTEGER_FAMILY{"tinyint", "smallint", "integer", "bigint", "int4", "long8"};
const std::set<std::string> DataType::NUMBER_FAMILY{"tinyint", "smallint", "integer", "bigint", "int4",   "long8",
                                                    "float",   "double",   "decimal", "real",   "numeric"};
const std::set<std::string> DataType::STRING_FAMILY{"varchar", "char"};

DataType::DataType(const std::string& name, int precision, int scale)
    : name(name), precision(precision), scale(scale) {}

DataType::DataType(const std::string& dataType) {
    name = dataType;
    precision = -1;
    scale = -1;

    // kylin's default precision
    if (name.compare("char") == 0) {
        precision = 255;
    } else if (name.compare("varchar") == 0) {
        precision = 256;
    } else if (name.compare("decimal") == 0 || name.compare("numeric") == 0) {
        precision = 19;
        scale = 4;
    }
}

DataType::~DataType() {}

DataType* DataType::getType(const std::string& type) {
    if (type.empty())
        return NULL;

    DataType* dataType = new DataType(type);
    return dataType;
}

bool DataType::isStringFamily() {
    if (STRING_FAMILY.find(name) != STRING_FAMILY.end())
        return true;
    else
        return false;
}

bool DataType::isIntegerFamily() {
    if (INTEGER_FAMILY.find(name) != INTEGER_FAMILY.end())
        return true;
    else
        return false;
}

bool DataType::isNumberFamily() {
    if (NUMBER_FAMILY.find(name) != NUMBER_FAMILY.end())
        return true;
    else
        return false;
}

bool DataType::isTinyInt() { return name.compare("tinyint") == 0; }

bool DataType::isSmallInt() { return name.compare("smallint") == 0; }

bool DataType::isInt() { return name.compare("int") == 0; }

bool DataType::isBigInt() { return name.compare("bigint") == 0; }

bool DataType::isFloat() { return name.compare("float") == 0; }

bool DataType::isDouble() { return name.compare("double") == 0; }

bool DataType::isDecimal() { return name.compare("decimal") == 0; }

bool DataType::isBoolean() { return name.compare("boolean") == 0; }

std::string DataType::getName() { return name; }

int DataType::getPrecision() { return precision; }

int DataType::getScale() { return scale; }
