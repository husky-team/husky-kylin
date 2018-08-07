#include <vector>;
#include <string>;

class DimensionDesc
{
public:
	DimensionDesc(std::string name, std::string table, std::string column);
	~DimensionDesc();
	
private:
	// from json
	std::string name;
	std::string table;
	std::string column;
	// std::vector<std::string> derived;

	// computed attributes
	TableRef * tableRef;
	TblColRef* columnRef; // suppose no derived columns
	// std::vector<TblColRef* > columnRefs; 
};