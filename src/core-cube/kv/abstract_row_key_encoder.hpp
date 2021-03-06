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

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "core-cube/cuboid/cuboid.hpp"

namespace husky {
namespace cube {

class CubeDesc;
class RowKeyEncoder;

class AbstractRowKeyEncoder {
   public:
    const unsigned char DEFAULT_BLANK_BYTE = 0xff;

    inline void set_blank_byte(unsigned char blank_byte) { blank_byte_ = blank_byte; }
    inline uint64_t get_cuboid_id() const { return cuboid_->get_id(); }
    inline void set_cuboid(const std::shared_ptr<Cuboid>& cuboid) { cuboid_ = cuboid; }

    virtual std::vector<unsigned char> create_buf() = 0;
    virtual void encode(const std::vector<unsigned char>& body_bytes, std::vector<unsigned char>& output_buf) = 0;
    virtual std::vector<unsigned char> encode(const std::map<std::shared_ptr<TblColRef>, std::string>& value_map) = 0;
    virtual std::vector<unsigned char> encode(std::vector<std::string>& values) = 0;
    virtual std::vector<unsigned char> encode(const std::vector<const std::string*>& values) = 0;

   protected:
    explicit AbstractRowKeyEncoder(const std::shared_ptr<Cuboid>& cuboid)
        : cuboid_(cuboid), blank_byte_(DEFAULT_BLANK_BYTE) {}
    ~AbstractRowKeyEncoder() {}

    unsigned char blank_byte_;
    std::shared_ptr<Cuboid> cuboid_;
};

}  // namespace cube
}  // namespace husky
