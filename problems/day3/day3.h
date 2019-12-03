#pragma once

#include <istream>
#include <vector>

namespace Day3 {
    using Point = std::pair<int, int>;
    using Wire = std::vector<Point>;

    Wire readWire(std::istream& input);
    int solveBoth(std::istream& input, bool part2);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
