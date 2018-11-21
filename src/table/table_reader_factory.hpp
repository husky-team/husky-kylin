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

#pragma once

#include "table/abstract_table_reader.hpp"
#include "table/orc_table_reader.hpp"
#include "table/text_table_reader.hpp"

namespace husky {
namespace table {

struct TableReaderFactory {
    static std::unique_ptr<AbstractTableReader> get_table_reader(const std::string& table_format) {
        std::unique_ptr<AbstractTableReader> reader;
#ifdef WITH_ORC
        if (table_format == "ORC") {
            return std::make_unique<husky::table::ORCTableReader>();
        }
#endif
        return std::make_unique<TextTableReader>();
    }
};

}  //  namespace husky
}  //  namespace table
