#pragma once

#include <istream>
#include <array>
#include <intcode.h>
#include <infinite_array.h>

using int_t = long long int;
using Program = intcode::Program<int_t, infinite_array<int_t>>;

namespace Day25 {
    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
