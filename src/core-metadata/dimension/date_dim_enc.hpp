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
#include "core-metadata/metadata/datatype/data_type.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

// TODO(tatiana): support YYYY-MM-DD format only for now
class DateDimEnc : public DimensionEncoding {
   public:
    explicit DateDimEnc(const std::vector<std::string>& enc_args) : codec_(enc_args) {}
    static const char ENCODING_NAME[];

    /** return the fixed length of encoded bytes */
    int get_length_of_encoding() override { return fixed_len_; }

    /** encode given value to bytes, note the NULL convention */
    inline void encode(const std::string& value, std::vector<unsigned char>& output, int output_offset) override {
        utils::write_long(codec_.value_to_code(value), output, output_offset, fixed_len_);
    }

    /** decode given bytes to value string, note the NULL convention */
    inline std::string decode(std::vector<unsigned char> bytes, int offset, int len) override {
        return codec_.code_to_value(utils::read_long(bytes, offset, fixed_len_));
    }

   private:
    class DateDimValueCodec {
       public:
        explicit DateDimValueCodec(const std::vector<std::string>& enc_args);

        inline uint64_t value_to_code(const std::string& value) {
            return get_num_days_since_0000_from_seconds(get_epoch_time(value));
        }
        std::string code_to_value(uint64_t code);

       private:
        // 86400 = 24 * 60 * 60 seconds in a day
        inline int get_num_days_since_0000_from_seconds(uint64_t time) { return time / 86400 + 719530; }
        inline uint64_t get_seconds_from_num_days_since_0000(int days) { return (days - 719530) * 86400; }

        uint64_t get_epoch_time(const std::string& date);

        std::shared_ptr<DataType> datatype_;
    };

    int fixed_len_ = 3;
    DateDimValueCodec codec_;
};

}  // namespace cube
}  // namespace husky
