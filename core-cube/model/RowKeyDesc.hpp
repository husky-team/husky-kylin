#include <string>
#include <vector>

#include "core-cube/model/CubeDesc.hpp"
#include "core-cube/model/RowKeyColDesc.hpp"
#include "core-metadata/metadata/model/TblColRef.hpp"

class RowKeyDesc {
   public:
    RowKeyDesc(CubeDesc* cubeDesc);
    ~RowKeyDesc();
    std::vetcor<RowKeyColDesc*> getRowKeyColumns();
    void setCubeDesc(CubeDesc* cubeDesc);
    int getColumnBitIndex(TblColRef* col);
    RowKeyColDesc* getColDesc(TblColRef* col);
    void setRowKeyColumns(const std::vector<RowKeyColDesc*>& rowKeyColumns);
    long getFullMask();
    vector<int> getColumnsNeedIndex();

   private:
    std::vector<RowKeyColDesc*> rowKeyColumns;
    long fullMask;
    std::map<TblColRef*, RowKeyColDesc*> columnMap;
    std::vector<int> columnsNeedIndex;

    void initColumnsNeedIndex();
    void buildRowKey();
};
