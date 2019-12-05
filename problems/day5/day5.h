#pragma once

#include <istream>
#include <vector>

namespace Day5 {
    int solve1(std::istream& input);
    int solve2(std::istream& input);

    std::vector<int> readProgram(std::istream& input);
    int runProgram(std::vector<int>& program, int input);
}
