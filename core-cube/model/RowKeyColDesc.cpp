#include "kylin/rowKeyCol.hpp"

RowKeyColDesc::RowKeyColDesc(std::string column, std::string encoding) {
    this->column = column;
    this->encoding = encoding;
}

void RowKeyColDesc::init(int index, CubeDesc cubeDesc) {
    bitIndex = index;
    colRef = cubeDesc.getModel().findColumn(column);
    column = colRef.getIdentity();

    std::vector<std::string> encodingParams = DimensionDimEnc::parseEncodingConf(encoding);
    if (encodingParams.size() == 1) {
        // no args
        encodingName = encodingParams[0];
    } else {
        encodingName = encodingParams[0];
        encodingArgs = encodingParams[1]
    }
}

std::string getEncoding() { return encoding; }

void setEncoding(std::string encoding) { this->encoding = encoding; }

std::string getColumn() { return column; }

void setColumn(std::string column) { this->column = column; }

std::string getIndex() { return index; }

void setIndex(std::string index) { this->index = index; }

std::string getEncodingName() { return encodingName; }

std::string getEncodingArgs() { return encodingArgs; }

int getBitIndex() { return bitIndex; }

TblColRef* getColRef() { return colRef; }