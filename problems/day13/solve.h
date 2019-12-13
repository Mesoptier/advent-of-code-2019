#pragma once

#include <istream>
#include <utility>
#include <vector>
#include <map>
#include <queue>

namespace Day13 {
    struct Coord {
        int x;
        int y;

        bool operator==(const Coord& rhs) const {
            return x == rhs.x &&
                y == rhs.y;
        }
        bool operator!=(const Coord& rhs) const {
            return !(rhs == *this);
        }
        bool operator<(const Coord& rhs) const {
            if (x < rhs.x)
                return true;
            if (rhs.x < x)
                return false;
            return y < rhs.y;
        }
        bool operator>(const Coord& rhs) const {
            return rhs < *this;
        }
        bool operator<=(const Coord& rhs) const {
            return !(rhs < *this);
        }
        bool operator>=(const Coord& rhs) const {
            return !(*this < rhs);
        }
    };

    using Grid = std::map<Coord, int>;

    void renderGrid(const Grid& grid, int score);

    struct Program {
        using Int = long long int;

        std::map<Int, Int> memory;
        Int ip = 0; // Instruction pointer
        Int rb = 0; // Relative base
        std::queue<Int> output;
        std::queue<Int> input;
        bool halted = false;

        explicit Program(std::istream& input);

        Int& at(Int index);

        bool run();
    };

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
