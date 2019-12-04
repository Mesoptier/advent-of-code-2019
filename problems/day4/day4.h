#pragma once

#include <istream>

namespace Day4 {
    bool checkPassword(int password, bool part2);
    int solveBoth(std::istream& input, bool part2);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
