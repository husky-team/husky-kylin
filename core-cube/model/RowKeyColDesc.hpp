#include <string>
#include <vector>

#include "kylin/cubeDesc.hpp"
#include "kylin/dimensionDimEnc.hpp"
#include "kylin/tblColRef.hpp"

class RowKeyColDesc {
   public:
    RowKeyColDesc(std::string column, std::string encoding);
    ~RowKeyColDesc();
    void init(int index, CubeDesc cubeDesc);
    std::string getEncoding();
    void setEncoding(std::string encoding);
    std::string getColumn();
    void setColumn(std::string column);
    std::string getIndex();
    void setIndex(std::string index);
    std::string getEncodingName();
    std::string getEncodingArgs();
    int getBitIndex();
    TblColRef* getColRef();

   private:
    std::string column;
    std::string encoding;
    std::string index;

    // computed attributes (but only encoding integers for now)
    std::string encodingName;
    std::string encodingArgs;  // interger size
    // std::vector<std::string> encodingArgs;
    int bitIndex;
    TblColRef* colRef;
};