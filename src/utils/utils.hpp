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

#include <cinttypes>
#include <vector>

namespace husky {
namespace utils {

std::vector<unsigned char> int_to_bytes(int param_int);

int bytes_to_int(const std::vector<unsigned char>& bytes);

std::vector<unsigned char> long_to_bytes(uint64_t param_long);

uint64_t bytes_to_long(const std::vector<unsigned char>& bytes);

void write_long(uint64_t num, std::vector<unsigned char>& bytes, int offset, int size);

uint64_t read_long(const std::vector<unsigned char>& bytes, int offset, int size);

int bit_count(uint64_t n);

uint64_t highest_one_bit(uint64_t n);
uint64_t number_of_leading_zeros(uint64_t n);

}  // namespace utils
}  // namespace husky
