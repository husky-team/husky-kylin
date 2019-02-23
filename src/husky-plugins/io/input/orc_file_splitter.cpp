// Copyright 2018 Husky Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef WITH_ORC
#include "husky-plugins/io/input/orc_file_splitter.hpp"

#include <string>
#include <list>
#include <vector>

#include "orc/ColumnPrinter.hh"
#include "orc/OrcFile.hh"
#include "orc/Vector.hh"
#include "orc/Exceptions.hh"
#include "orc/orc-config.hh"

#include "base/log.hpp"
#include "base/serialization.hpp"
#include "core/constants.hpp"
#include "core/context.hpp"
#include "core/coordinator.hpp"

#include "husky-plugins/core/constants.hpp"
#include "husky-plugins/io/input/orc_hdfs_inputstream.hpp"
#include "husky-plugins/nlohmann/json.hpp"

namespace orc {

// This class is used to print records row by row without field names for each rows.
// Within each row record, fields are segregated by a tab.
class HDFSColumnPrinter : public ColumnPrinter {
   public:
    HDFSColumnPrinter(std::string& buf, const Type& type) : ColumnPrinter(buf) {
        numOfFields = type.getSubtypeCount();
        for (unsigned int i = 0; i < numOfFields; i++) {
            fieldPrinter.push_back(createColumnPrinter(buf, type.getSubtype(i)).release());
            fieldTypes.push_back(type.getSubtype(i)->getKind());
        }
        using json = nlohmann::json;
        std::ifstream input_json(husky::Context::get_param("input"));
        json j_obj;
        input_json >> j_obj;
        for(json::iterator it  = j_obj["projection"].begin(); it != j_obj["projection"].end(); ++it)
            proj_ids_.push_back((*it)["id"]);
    }

    virtual ~HDFSColumnPrinter() {
        for (size_t i = 0; i < fieldPrinter.size(); i++) {
            delete fieldPrinter[i];
        }
    }



    void printRow(uint64_t rowId) override {
        if (hasNulls && !notNull[rowId]) {
            writeString(buffer, "null");
        } else {
            for (std::vector<int>::iterator it = proj_ids_.begin(); it != proj_ids_.end(); ++it) {
                if (*it != 0) {
                    writeString(buffer, "\t");
                }
                fieldPrinter[*it]->printRow(rowId);
            }
        }
    }

    void reset(const ColumnVectorBatch& batch) override {
        ColumnPrinter::reset(batch);
        const StructVectorBatch& structBatch = dynamic_cast<const StructVectorBatch&>(batch);
        for (size_t i = 0; i < fieldPrinter.size(); i++) {
            fieldPrinter[i]->reset(*(structBatch.fields[i]));
        }
    }

   private:
    void writechar(std::string& file, char ch) { file += ch; }

    void writeString(std::string& file, const char* ptr) {
        size_t len = strlen(ptr);
        file.append(ptr, len);
    }
    int numOfFields;
    std::vector<ColumnPrinter*> fieldPrinter;
    std::vector<int> fieldTypes;
    std::vector<int> proj_ids_;
};

}  // namespace orc

namespace husky {
namespace io {

using orc::ReaderOptions;
using orc::createReader;
using orc::readLocalFile;
using orc::ColumnVectorBatch;

ORCFileSplitter::ORCFileSplitter() { offset_ = 0; }
ORCFileSplitter::~ORCFileSplitter() {
    if (protocol_ == "hdfs")
        hdfsDisconnect(fs_);
}

void ORCFileSplitter::load(std::string url) {
    cur_fn_ = "";
    url_ = url;
    struct hdfsBuilder* builder = hdfsNewBuilder();
    hdfsBuilderSetNameNode(builder, husky::Context::get_param("hdfs_namenode").c_str());
    hdfsBuilderSetNameNodePort(builder, std::stoi(husky::Context::get_param("hdfs_namenode_port")));
    fs_ = hdfsBuilderConnect(builder);
    hdfsFreeBuilder(builder);

    //Read projection list from JSON
    std::ifstream input_json(husky::Context::get_param("input"));
    using json = nlohmann::json;
    json j_obj;
    input_json >> j_obj;
    conds_ = j_obj["condition"];
}

// ask master for offset and url
// we are not using fn for now since url is not a directory
boost::string_ref ORCFileSplitter::fetch_block(bool is_next) {
    BinStream question;
    question << url_;
    BinStream answer = husky::Context::get_coordinator()->ask_master(question, husky::TYPE_ORC_BLK_REQ);
    std::string fn;
    answer >> fn;
    answer >> offset_;
    if (fn == "") {
        return "";
    } else if (fn != cur_fn_) {
        cur_fn_ = fn;
        ReaderOptions opts;
        reader_ = createReader(read_hdfs_file(fs_, cur_fn_), opts);
    }
    return read_by_batch(offset_);
}

void debug_json(nlohmann::json conds) {
  std::cout << "opt: " << conds["operator"] << "  value: " << conds["value"] << "  num_opts: " << conds["num_of_opds"] << std::endl;
}

void display_info(std::string str) {
  std::cout << str << std::endl;
}

bool ORCFileSplitter::computeBoolRes(nlohmann::json conds, orc::ColumnVectorBatch& batch, int rowId, int num_of_fields) {
    //Read projection list from JSON
    using json = nlohmann::json;
    using ColumnVectorBatch = orc::ColumnVectorBatch;
    using LongVectorBatch = orc::LongVectorBatch;
    using DoubleVectorBatch = orc::DoubleVectorBatch;
    int num_of_opds = conds["num of operand"];
    std::string opt = conds["operator"];

    if(opt == "<" || opt == "=" || opt == ">") {
        json opd1 = conds["operand 1"];
        int curId1 = opd1["id"];
        json opd2 = conds["operand 2"];
        int curId2 = opd2["id"];

        std::string value1 = "";
        std::string value2 = "";

        if(curId1 > num_of_fields) {
            value1 = opd1["value"];

            if(curId2 <= num_of_fields) {
                ColumnVectorBatch* sub_vector = dynamic_cast<const orc::StructVectorBatch&>(batch).fields[curId2];
                bool has_null = sub_vector->hasNulls;
                if(has_null) {
                    const char* not_nulls = sub_vector->notNull.data();
                    if(!not_nulls[rowId])
                        return false;
                }

                //Int or Long
                if(fieldTypes_[curId2] == 3 || fieldTypes_[curId2] == 4) {
                    int64_t constant = std::stol(value1, nullptr, 10);
                    const int64_t* data = dynamic_cast<const LongVectorBatch&>(*sub_vector).data.data();
                    int64_t long_data = static_cast<int64_t>(data[rowId]);
                    if(opt == "<") {
                        if(constant < long_data)
                          return true;
                        else
                          return false;
                    } else if(opt == ">") {
                        if(constant > long_data)
                          return true;
                        else
                          return false;
                    } else {
                        if(constant == long_data)
                          return true;
                        else
                          return false;
                    }
                } 
                //Float or Double 
                else if(fieldTypes_[curId2] == 5 || fieldTypes_[curId2] == 6) {
                    double constant = std::stod(value1, nullptr);
                    const double* data = dynamic_cast<const DoubleVectorBatch&>(*sub_vector).data.data();
                    double long_data = data[rowId];
                    if(opt == "<") {
                        if(constant < long_data)
                          return true;
                        else
                          return false;
                    } else if(opt == ">") {
                        if(constant > long_data)
                          return true;
                        else
                          return false;
                    } else {
                        if(constant == long_data)
                          return true;
                        else
                          return false;
                    }
                }
                //String
                else if(fieldTypes_[curId2] == 7 || fieldTypes_[curId2] == 8 || fieldTypes_[curId2] == 16 || fieldTypes_[curId2] == 17) {
                    std::string& constant = value1;
                    const char* const * start = dynamic_cast<const orc::StringVectorBatch&>(*sub_vector).data.data();
                    const int64_t* length = dynamic_cast<const orc::StringVectorBatch&>(batch).length.data();
                    std::string string_data = "";
                    for(int64_t i=0; i < length[rowId]; ++i) {
                        char ch = static_cast<char>(start[rowId][i]);
                        switch (ch) {
                            case '\\':
                                string_data.append("\\\\");
                                break;
                            case '\b':
                                string_data.append("\\b");
                                break;
                            case '\f':
                                string_data.append("\\f");
                                break;
                            case '\n':
                                string_data.append( "\\n");
                                break;
                            case '\r':
                                string_data.append("\\r");
                                break;
                            case '\t':
                                string_data.append("\\t");
                                break;
                            case '"':
                                string_data.append("\\\"");
                                break;
                            default:
                                string_data.append(1, ch);
                                break;
                       }
                   
                    }
                    if(opt == "<") {
                        if(constant < string_data)
                          return true;
                        else
                          return false;
                    } else if(opt == ">") {
                        if(constant > string_data)
                          return true;
                        else
                          return false;
                    } else {
                        if(constant == string_data)
                          return true;
                        else
                          return false;
                    }
                }

            } 
        } 
        else {
            value2 = opd2["value"];
            if(curId1 <= num_of_fields) {
               
                ColumnVectorBatch* sub_vector = dynamic_cast<const orc::StructVectorBatch&>(batch).fields[curId1];

                bool has_null = sub_vector->hasNulls;
                if(has_null) {
                    const char* not_nulls = sub_vector->notNull.data();
                    if(!not_nulls[rowId])
                        return false;
                }

                if(fieldTypes_[curId1] >= 0 && fieldTypes_[curId1] <= 4) {
                    int64_t constant = std::stol(value2, nullptr, 10);

                    const int64_t* data = dynamic_cast<const LongVectorBatch&>(*sub_vector).data.data();
                    int64_t long_data = static_cast<int64_t>(data[rowId]);
                    if(opt == "<") {
                        if(long_data < constant)
                          return true;
                        else
                          return false;
                    } else if(opt == ">") {
                        if(long_data > constant)
                          return true;
                        else
                          return false;
                    } else {
                        if(constant == long_data)
                          return true;
                        else
                          return false;
                    }
                } 
                else if(fieldTypes_[curId1] == 5 || fieldTypes_[curId1] == 6) {
                    double constant = std::stod(value2, nullptr);
                    const double* data = dynamic_cast<const DoubleVectorBatch&>(*sub_vector).data.data();
                    double long_data = data[rowId];
                    if(opt == "<") {
                        if(long_data < constant)
                          return true;
                        else
                          return false;
                    } else if(opt == ">") {
                        if(long_data > constant)
                          return true;
                        else
                          return false;
                    } else {
                        if(constant == long_data)
                          return true;
                        else
                          return false;
                    }
                }
                //String
                else if(fieldTypes_[curId1] == 7 || fieldTypes_[curId1] == 8 || fieldTypes_[curId1] == 16 || fieldTypes_[curId1] == 17) {
                    std::string& constant = value1;
                    const char* const * start = dynamic_cast<const orc::StringVectorBatch&>(*sub_vector).data.data();
                    const int64_t* length = dynamic_cast<const orc::StringVectorBatch&>(batch).length.data();
                    std::string string_data = "";
                    for(int64_t i=0; i < length[rowId]; ++i) {
                        char ch = static_cast<char>(start[rowId][i]);
                        switch (ch) {
                            case '\\':
                                string_data.append("\\\\");
                                break;
                            case '\b':
                                string_data.append("\\b");
                                break;
                            case '\f':
                                string_data.append("\\f");
                                break;
                            case '\n':
                                string_data.append( "\\n");
                                break;
                            case '\r':
                                string_data.append("\\r");
                                break;
                            case '\t':
                                string_data.append("\\t");
                                break;
                            case '"':
                                string_data.append("\\\"");
                                break;
                            default:
                                string_data.append(1, ch);
                                break;
                       }
                   
                    }
                    if(opt == "<") {
                        if(constant < string_data)
                          return true;
                        else
                          return false;
                    } else if(opt == ">") {
                        if(constant > string_data)
                          return true;
                        else
                          return false;
                    } else {
                        if(constant == string_data)
                          return true;
                        else
                          return false;
                   }
               } 
            }              
        } 
    }
    else {
        for(int i = 0; i < num_of_opds; i++) {
            std::string opd_name = "operand ";
            char idx = '0' + i + 1;
            opd_name.push_back(idx);
            json cur_opd = conds[opd_name];
            bool cur_res = computeBoolRes(cur_opd, batch, rowId, num_of_fields);

            if(opt == "OR") {
                if(cur_res == true)
                    return true;
            } else if(opt == "AND") {
                if(cur_res == false)
                    return false;
            }
        }
        if(opt == "OR")
          return false;
        else if(opt == "AND")
          return true;
    }  
}


boost::string_ref ORCFileSplitter::read_by_batch(size_t offset) {
    // if (protocol_ == "hdfs") {
    buffer_.clear();
    try {
        std::string line = "";
        orc::RowReaderOptions row_opts;
        //row_opts.include(proj_ids);
        std::unique_ptr<orc::RowReader> row_reader = reader_->createRowReader(row_opts);
        row_reader->seekToRow(offset);
        std::unique_ptr<orc::ColumnPrinter> printer(new orc::HDFSColumnPrinter(line, row_reader->getSelectedType()));
        std::unique_ptr<ColumnVectorBatch> batch = row_reader->createRowBatch(kOrcRowBatchSize);
        int num_of_fields = row_reader->getSelectedType().getSubtypeCount();
        for (unsigned int i = 0; i < num_of_fields; i++) {
            fieldTypes_.push_back(row_reader->getSelectedType().getSubtype(i)->getKind());
        }
        if (row_reader->next(*batch)) {
            printer->reset(*batch);
            for (unsigned int i = 0; i < batch->numElements; ++i) {
                bool check_conds = computeBoolRes(conds_, (*batch), i, num_of_fields);
                if(check_conds == false)
                    continue;
                line.clear();
                printer->printRow(i);
                buffer_ += line + "\n";
            }
        }
    } catch (const std::exception& e) {
        LOG_I << e.what();
    }
    return boost::string_ref(buffer_);
    // } else if (protocol_ == "nfs") {
    /*   try {
           std::string line = "";

           orc::RowReaderOptions row_opts;
           ORC_UNIQUE_PTR<orc::RowReader> row_reader = reader_->createRowReader(row_opts);
           row_reader->seekToRow(offset_);
           std::unique_ptr<orc::ColumnPrinter> printer = createColumnPrinter(line, &row_reader->getSelectedType());
           std::unique_ptr<ColumnVectorBatch> batch = row_reader->createRowBatch(kOrcRowBatchSize);
           if (!buffer_.empty()) {
               buffer_.clear();
           }
           if (row_reader->next(*batch)) {
               printer->reset(*batch);
               unsigned long i;
               for (i = 0; i < batch->numElements; ++i) {
                   line.clear();
                   printer->printRow(i);
                   line += "\n";
                   buffer_ += line;
               }
           }
       } catch (const std::exception& e) {
           base::log_msg(e.what());
       }
   } else {
       base::log_msg("Protocol:" + protocol_ + " is not suported");
       return;
   }*/
}

}  // namespace io
}  // namespace husky
#endif
