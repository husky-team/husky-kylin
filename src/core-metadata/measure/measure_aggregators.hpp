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

#include "core-metadata/measure/measure_type.hpp"
#include "core-metadata/metadata/model/measure_desc.hpp"

namespace husky {
namespace cube {

class MeasureAggregators {
   public:
    using SValT = std::vector<std::string>;
    explicit MeasureAggregators(const std::vector<std::shared_ptr<MeasureDesc>>& measure_descs) {
        // TODO(tatiana): back dependent aggregator?
        for (auto& measure : measure_descs) {
            aggregators_.push_back(measure->get_function()->get_measure_type()->new_aggregator());
        }
    }

    inline size_t size() const { return aggregators_.size(); }

    void aggregate(SValT& val, const SValT& inc) const {
        for (int i = 0; i < inc.size(); ++i) {
            aggregators_[i]->aggregate(val[i], inc[i]);
        }
    }

   private:
    std::vector<std::shared_ptr<MeasureAggregator>> aggregators_;
};

}  // namespace cube
}  // namespace husky
