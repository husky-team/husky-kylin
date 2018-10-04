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

#include "boost/algorithm/string.hpp"

namespace husky {
namespace cube {

class DimensionDimEnc {
   public:
    // it's convention that all 0xff means NULL
    static const unsigned char kNULL = (unsigned char) 0xff;

    static bool is_null(std::vector<unsigned char> bytes, int offset, int length) {
        // all 0xff is NULL
        if (length == 0) {
            return false;
        }
        for (int i = 0; i < length; i++) {
            if (bytes[i + offset] != kNULL)
                return false;
        }
        return true;
    }

    static std::vector<std::string> parse_encoding_conf(const std::string& encoding) {
        std::vector<std::string> results;
        // boost::algorithm::split(results, encoding, is_any_of(":"));
        int cut = encoding.rfind(':');
        if (cut == std::string::npos) {
            // no args
            results.push_back(encoding);
        } else {
            // this is an arg
            results.push_back(encoding.substr(0, cut));
            results.push_back(encoding.substr(cut + 1));
        }
        return results;
    }

    /** return the fixed length of encoded bytes */
    virtual int get_length_of_encoding() = 0;

    /** encode given value to bytes, note the NULL convention */
    virtual void encode(const std::string& value, std::vector<unsigned char> output, int outputOffset) = 0;

    /** decode given bytes to value string, note the NULL convention */
    virtual std::string decode(std::vector<unsigned char> bytes, int offset, int len) = 0;

    /** return a DataTypeSerializer that does the same encoding/decoding on ByteBuffer */
    // virtual DataTypeSerializer<Object> asDataTypeSerializer();
};

}  // namespace cube
}  // namespace husky
