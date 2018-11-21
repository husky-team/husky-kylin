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
#include <vector>

namespace husky {
namespace cube {

class CuboidKV {
   public:
    using KeyT = std::vector<unsigned char>;
    using ValT = std::vector<std::string>;

    explicit CuboidKV(const KeyT& key) : key_(key) {}
    CuboidKV(KeyT&& key, ValT&& val) : key_(std::move(key)), measure_strs_(std::move(val)) {}

    inline const KeyT& id() const { return key_; }
    inline void set_measure_strs(ValT&& val) { measure_strs_ = std::move(val); }
    inline void set_measure_strs(const ValT& val) { measure_strs_ = val; }
    inline const ValT& get_measure_strs() const { return measure_strs_; }

   private:
    KeyT key_;
    ValT measure_strs_;
};

}  // namespace cube
}  // namespace husky
