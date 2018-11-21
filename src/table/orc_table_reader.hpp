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
#pragma once

#include <cstring>
#include <string>
#include <vector>

#include "boost/utility/string_ref.hpp"

#include "base/log.hpp"
#include "core/engine.hpp"

#include "husky-plugins/io/input/orc_inputformat.hpp"
#include "table/abstract_table_reader.hpp"

namespace husky {
namespace table {

class ORCTableReader : public AbstractTableReader {
   public:
    virtual ~ORCTableReader() {}
    inline uint32_t get_row_num() const override { return row_num_; }
    inline uint32_t get_column_num() const override { return column_num_; }
    inline std::vector<std::vector<std::string>> get_data() override { return std::move(raw_data_); }

    /*
     * Read ORC format table raw data
     *
     * @param url table file path
     * TODO(tatiana): input format interface for better io locality
     */
    void read_data(const std::string& url) override {
        io::ORCInputFormat infmt;
        infmt.set_input(url);

        auto read_line = [this](boost::string_ref& line) {
            raw_data_.push_back(cut(line));
            ++row_num_;
        };

        husky::load(infmt, read_line);
    }

   protected:
    std::vector<std::string> cut(boost::string_ref& line) {
        std::vector<std::string> ret;
        ret.reserve(column_num_ + 1);

        char* strtok_pos;
        std::shared_ptr<char> record_ptr(new char[line.size() + 1], [](char* p) { delete[] p; });
        strncpy(record_ptr.get(), line.data(), line.size());
        record_ptr.get()[line.size()] = '\0';
        char* tok = strtok_r(record_ptr.get(), "\t", &strtok_pos);
        while (tok != NULL) {
            ret.emplace_back(tok);
            tok = strtok_r(NULL, "\t", &strtok_pos);
        }
        column_num_ = ret.size();

        return ret;
    }

   private:
    std::vector<std::vector<std::string>> raw_data_;
    uint32_t row_num_ = 0;
    uint32_t column_num_ = 0;
};

}  // namespace table
}  // namespace husky
#endif
