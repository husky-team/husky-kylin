#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include <set>
#include <string>

class DataType
{
	// support integer, number(float family) and string
public:
	DataType(const std::string & name, int precision, int scale);
	~DataType();
	const static std::set<std::string> INTEGER_FAMILY = {"tinyinyt", "smallint", "integer", "bigint", "int4", "long8"};
	const static std::set<std::string> NUMBER_FAMILY = {"tinyinyt", "smallint", "integer", "bigint", "int4", "long8", "float", "double", "decimal", "real", "numeric"};
	const static std::set<std::string> STRING_FAMILY = {"varchar", "char"};
	static DataType * getType(std::string type);

private:
	std::string name;
	int precision;
	int scale;
};

#endif