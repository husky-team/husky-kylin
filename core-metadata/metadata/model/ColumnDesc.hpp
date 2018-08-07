#ifndef _COLUMN_DESC_H_
#define _COLUMN_DESC_H_

#include <string>

// #include "nlohmann/json.hpp"

// using json = nlohmann::json;

class ColumnDesc
{
public:
	ColumnDesc(const std::string& id, const std::string& name, const std::string& datatype);
	~ColumnDesc();
	std::string getId();
	std::string getName();
	std::string getDatatype();


private:
	std::string id;
	std::string name;
	std::string datatype;
};

#endif