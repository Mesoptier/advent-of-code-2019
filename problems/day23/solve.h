#pragma once

#include <istream>
#include <intcode.h>

using int_t = long long int;
using Program = intcode::Program<int_t, std::array<int_t, 4096>>;

namespace Day23 {
    int_t solve1(std::istream& input);
    int solve2(std::istream& input);
}
