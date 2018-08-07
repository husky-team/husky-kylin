#ifndef _PARAMETER_DESC_H_
#define _PARAMETER_DESC_H_

#include <string>

class ParameterDesc
{
public:
	ParameterDesc(const std::string & type, const std::string & value);
	~ParameterDesc();
	std::string getType();
	void setType(std::string type);
	std::string getValue();
	void setValue(std::string value);
	TblColRef * getColRef();
	void setTblColRef(TblColRef * colRef);
	bool isColumnTyoe();
	
private:
	std::string type;
	std::string value;

	// computed attributes
	TblColRef * colRef = NULL;
};

#endif