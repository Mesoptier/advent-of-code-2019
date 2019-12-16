#pragma once

#include <istream>
#include <vector>
#include <map>
#include <queue>

namespace Day16 {
    std::vector<int> read_signal(std::istream& input);
    int get_digits(const std::vector<int>& signal, int offset, int length);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
