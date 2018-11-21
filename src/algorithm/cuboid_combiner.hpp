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
#include <vector>

#include "base/thread_support.hpp"

#include "core-metadata/measure/measure_aggregators.hpp"

namespace husky {
namespace cube {

struct CuboidStringCombiner {
    using ValT = std::vector<std::string>;

    static void combine(ValT& val, const ValT& inc) {
        CHECK_EQ(inc.size(), aggs_->size()) << "inc size " << inc.size() << " vs. aggregators size " << aggs_->size();
        aggs_->aggregate(val, inc);
    }

    inline static void set_aggregators(const std::shared_ptr<MeasureAggregators>& aggs) {
        base::call_once_each_time([aggs = std::move(aggs)]() { aggs_ = aggs; });
    }

   private:
    static std::shared_ptr<MeasureAggregators> aggs_;
};

class CuboidByteCombiner {
   public:
    using ValT = std::vector<std::vector<unsigned char>>;
    static void combine(ValT& val, const ValT& inc) {
        // TODO(tatiana)
    }
};

}  // namespace cube
}  // namespace husky
