#pragma once

#include <istream>
#include "../../util/intcode.h"
#include "../../util/infinite_array.h"

using Program = intcode::Program<long long, infinite_array<long long>>;

namespace Day21 {
    long long int solve1(std::istream& input);
    long long int solve2(std::istream& input);
}
