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

#include <memory>
#include <string>

#include "core-metadata/metadata/model/data_model_desc.hpp"
#include "core-metadata/metadata/model/function_desc.hpp"

namespace husky {
namespace cube {

class MeasureDesc {
   public:
    MeasureDesc(const std::string& name, const std::shared_ptr<FunctionDesc>& function)
        : name_(name), function_(function) {}

    inline const std::string& get_name() const { return name_; }
    inline void set_name(const std::string& name) { name_ = name; }
    inline const std::shared_ptr<FunctionDesc>& get_function() const { return function_; }
    inline void set_function(const std::shared_ptr<FunctionDesc>& function) { function_ = function; }

    inline void init(const DataModelDesc& model) const { function_->init(model); }

   private:
    std::string name_;
    std::shared_ptr<FunctionDesc> function_;
};

}  // namespace cube
}  // namespace husky
