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

#include "utils/utils.hpp"

namespace std {

template <>
struct hash<std::vector<unsigned char>> {
    size_t operator()(const std::vector<unsigned char>& key) const {
        if (key.empty()) {
            return 0;
        }
        if (key.size() <= sizeof(uint64_t)) {
            size_t num = husky::utils::read_long(key, 0, key.size());
            return num ^ (num >> 32);
        }
        size_t hash = 1;
        for (auto c : key) {
            hash = 31 * hash + (size_t) c;
        }
        return hash;
    }
};

}  // namespace std
