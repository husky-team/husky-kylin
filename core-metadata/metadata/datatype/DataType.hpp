#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include <set>
#include <string>

class DataType
{
	// support integer, number(float family) and string
public:
	DataType(const std::string & name, int precision, int scale);
	DataType::DataType(const std::string & dataType);
	~DataType();
	// DATETIME_FAMILY
	const static std::set<std::string> INTEGER_FAMILY = {"tinyinyt", "smallint", "integer", "bigint", "int4", "long8"};
	const static std::set<std::string> NUMBER_FAMILY = {"tinyinyt", "smallint", "integer", "bigint", "int4", "long8", "float", "double", "decimal", "real", "numeric"};
	const static std::set<std::string> STRING_FAMILY = {"varchar", "char"};
	static DataType * getType(std::string type);
	bool isStringFamily();
	bool isIntegerFamily();
	bool isNumberFamily();
	bool isTinyInt();
	bool isSmallInt();
	bool isInt();
	bool isBigInt();
	bool isFloat();
	bool isDouble();
	bool isDecimal();
	bool isBoolean();
	std::string getName();
	int getPrecision();
	int getScale();

   private:
    std::string name;
    int precision;
    int scale;
};

#endif
