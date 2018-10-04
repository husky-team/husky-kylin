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

#include "core-metadata/dimension/integer_dim_enc.hpp"

#include <string>

#include <vector>
#include "glog/logging.h"

#include "utils/utils.hpp"

namespace husky {
namespace cube {

const std::vector<uint64_t> IntegerDimEnc::CAP = {
    0, 0x7fL, 0x7fffL, 0x7fffffL, 0x7fffffffL, 0x7fffffffffL, 0x7fffffffffffL, 0x7fffffffffffffL, 0x7fffffffffffffffL};
const std::vector<uint64_t> IntegerDimEnc::MASK = {
    0, 0xffL, 0xffffL, 0xffffffL, 0xffffffffL, 0xffffffffffL, 0xffffffffffffL, 0xffffffffffffffL, 0xffffffffffffffffL};
const std::vector<uint64_t> TAIL = {
    0, 0x80L, 0x8000L, 0x800000L, 0x80000000L, 0x8000000000L, 0x800000000000L, 0x80000000000000L, 0x8000000000000000L};

const char IntegerDimEnc::ENCODING_NAME[] = "integer";

IntegerDimEnc::IntegerDimEnc(int len) : fixed_len_(len) {
    CHECK(len > 0 && len < CAP.size()) << "[IntegerDimEnc] len " << len << " exceeds valid scope (0, " << CAP.size()
                                       << ")";
}

/** encode given value to bytes, note the NULL convention */
void IntegerDimEnc::encode(const std::string& valueStr, std::vector<unsigned char> output, int outputOffset) {
    // if (valueStr.empty()) {}
    uint64_t integer = std::stol(valueStr);
    utils::write_long(integer + CAP[fixed_len_], output, outputOffset, fixed_len_);
}

/** decode given bytes to value string, note the NULL convention */
std::string IntegerDimEnc::decode(std::vector<unsigned char> bytes, int offset, int len) {
    uint64_t integer = utils::read_long(bytes, offset, len) - CAP[fixed_len_];

    // only take useful bytes
    integer = integer & MASK[fixed_len_];
    bool positive = (integer & ((0x80L) << ((fixed_len_ - 1) << 3))) == 0;
    if (!positive) {
        integer |= (~MASK[fixed_len_]);
    }

    return std::to_string(integer);
}

}  // namespace cube
}  // namespace husky
