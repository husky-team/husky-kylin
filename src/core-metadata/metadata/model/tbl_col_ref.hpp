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

#include <string>

#include "core-metadata/metadata/model/column_desc.hpp"

namespace husky {
namespace cube {

class TableRef;

class TblColRef {
   public:
    explicit TblColRef(ColumnDesc* column);
    TblColRef(TableRef* table, ColumnDesc* column);
    ~TblColRef() {}

    ColumnDesc* get_column_desc() { return column_; }
    inline const std::string& get_name() const { return column_->get_name(); }
    inline const std::string& get_identity() const { return identity_; }
    inline const std::string& get_table_alias() const;

   private:
    TableRef* table_;     // not owned, this is my owner
    ColumnDesc* column_;  // not owned
    std::string identity_;
};

}  // namespace cube
}  // namespace husky
