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

#include "core-metadata/dimension/dimension_encoding.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

class IntegerDimEnc : public DimensionEncoding {
   public:
    explicit IntegerDimEnc(int len);
    ~IntegerDimEnc();
    static const char ENCODING_NAME[];

    /** return the fixed length of encoded bytes */
    virtual int get_length_of_encoding() { return fixed_len_; }

    /** encode given value to bytes, note the NULL convention */
    virtual void encode(const std::string& valueStr, std::vector<unsigned char> output, int outputOffset);

    /** decode given bytes to value string, note the NULL convention */
    virtual std::string decode(std::vector<unsigned char> bytes, int offset, int len);

   private:
    static const std::vector<uint64_t> CAP;
    static const std::vector<uint64_t> MASK;
    static const std::vector<uint64_t> TAIL;
    int fixed_len_;
};

}  // namespace cube
}  // namespace husky
