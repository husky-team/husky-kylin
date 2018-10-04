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

#include "core-metadata/metadata/model/measure_desc.hpp"

#include <memory>
#include <string>

#include "core-metadata/metadata/model/function_desc.hpp"

namespace husky {
namespace cube {

MeasureDesc::MeasureDesc(const std::string& name, std::shared_ptr<FunctionDesc>& function)
    : name_(name), function_(function) {
    // string cubeDescJsonPath = "cube_desc.json"; // should be in hdfs
    // std::ifstream ifs(cubeDescJsonPath);
    // json j = json::parse(ifs);
}

}  // namespace cube
}  // namespace husky
