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

#include "core-metadata/metadata/model/parameter_desc.hpp"

#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "core-metadata/metadata/model/function_desc.hpp"

namespace husky {
namespace cube {

ParameterDesc::ParameterDesc(const json& j_parameter)
    : type_(j_parameter["type"].get<std::string>()), value_(j_parameter["value"].get<std::string>()) {
    if (j_parameter.find("next_parameter") != j_parameter.end()) {
        next_parameter_ = std::make_unique<ParameterDesc>(j_parameter["next_parameter"]);
    }
}

bool ParameterDesc::is_column_type() const { return type_.compare(FunctionDesc::PARAMETER_TYPE_COLUMN) == 0; }

const std::vector<std::shared_ptr<TblColRef>>& ParameterDesc::get_col_refs() {
    if (!col_ref_collected) {
        col_ref_collected = true;
        auto p = this;
        while (p != nullptr) {
            if (p->is_column_type()) {
                all_col_refs_including_nexts_.push_back(p->get_col_ref());
            }
            p = p->next_parameter().get();
        }
    }
    return all_col_refs_including_nexts_;
}

}  // namespace cube
}  // namespace husky
