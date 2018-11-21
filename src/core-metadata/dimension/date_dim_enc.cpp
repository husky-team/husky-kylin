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

#include "core-metadata/dimension/date_dim_enc.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "core-metadata/metadata/datatype/data_type.hpp"
#include "utils/utils.hpp"

namespace husky {
namespace cube {

const char DateDimEnc::ENCODING_NAME[] = "date";

DateDimEnc::DateDimValueCodec::DateDimValueCodec(const std::vector<std::string>& enc_args) {
    if (enc_args.size() == 1) {
        datatype_ = DataType::get_type(enc_args.front());
    }
}

uint64_t DateDimEnc::DateDimValueCodec::get_epoch_time(const std::string& date) {
    // Consider all the input in this format: '2016-08-31'.
    static const auto dateTimeFormat = "%Y-%m-%d";

    std::tm t;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    strptime(date.c_str(), dateTimeFormat, &t);

    return std::mktime(&t);
}

std::string DateDimEnc::DateDimValueCodec::code_to_value(uint64_t code) {
    if (datatype_ != nullptr && datatype_->is_integer_family()) {
        time_t seconds = get_seconds_from_num_days_since_0000(code);
        std::tm time;
        localtime_r(&seconds, &time);
        std::stringstream ss;
        ss << std::put_time(&time, "%Y%m%d");
        return ss.str();
    }
    return std::to_string(get_seconds_from_num_days_since_0000(code));
}

}  // namespace cube
}  // namespace husky
