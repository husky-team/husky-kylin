#include <string>

#include "nlohmann/json.hpp"
#include "kylin/functionDesc.hpp"

using json = nlohmann::json;

class MeasureDesc
{
public:
	MeasureDesc(std::string name. FunctionDesc * function);
	~MeasureDesc();
	std::string getName();
	void setName(std::string name);
	FunctionDesc * getFunction();
	void setFunction(FunctionDesc * function);
	
private:
	std::string name;
	FunctionDesc * function;
};