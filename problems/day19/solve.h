#pragma once

#include <istream>
#include <utility>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>
#include "../../util/intcode.h"

using namespace intcode;

namespace Day19 {
    int solve1(std::istream& input);
    int solve2(std::istream& input);

    bool get_state(const Program<>& program, int x, int y);
}
