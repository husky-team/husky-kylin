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
        for (unsigned int i = 0; i < type.getSubtypeCount(); i++) {
            fieldPrinter.push_back(createColumnPrinter(buf, type.getSubtype(i)).release());
        }
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
            for (unsigned int i = 0; i < fieldPrinter.size(); i++) {
                if (i != 0) {
                    writeString(buffer, "\t");
                }
                fieldPrinter[i]->printRow(rowId);
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

    std::vector<ColumnPrinter*> fieldPrinter;
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
    proj_ids = std::list<uint64_t>();
    for(json::iterator it  = j_obj["projection"].begin(); it != j_obj["projection"].end(); ++it)
        proj_ids.push_back((*it)["id"]);
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

boost::string_ref ORCFileSplitter::read_by_batch(size_t offset) {
    // if (protocol_ == "hdfs") {
    buffer_.clear();
    try {
        std::string line = "";
        orc::RowReaderOptions row_opts;
        row_opts.include(proj_ids);
        std::unique_ptr<orc::RowReader> row_reader = reader_->createRowReader(row_opts);
        row_reader->seekToRow(offset);
        std::unique_ptr<orc::ColumnPrinter> printer(new orc::HDFSColumnPrinter(line, row_reader->getSelectedType()));
        std::unique_ptr<ColumnVectorBatch> batch = row_reader->createRowBatch(kOrcRowBatchSize);
        if (row_reader->next(*batch)) {
            printer->reset(*batch);
            for (unsigned int i = 0; i < batch->numElements; ++i) {
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
