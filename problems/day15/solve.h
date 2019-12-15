#pragma once

#include <istream>
#include <utility>
#include <vector>
#include <map>
#include <queue>

namespace Day15 {

    enum Direction {
        North = 1,
        South = 2,
        West = 3,
        East = 4,
    };

    Direction reverse(Direction direction);

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

        Coord step(Direction direction) const {
            switch (direction) {
                case North:
                    return { x, y - 1 };
                case South:
                    return { x, y + 1 };
                case West:
                    return { x - 1, y };
                case East:
                    return { x + 1, y };
            }
        }
    };

    using Grid = std::map<Coord, int>;

    void print_grid(const Grid& grid);

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

    Grid recover_grid(Program& program, const Coord& origin);
    void flood_fill(Program& program, Grid& grid, const Coord& current);
    int compute_cost(const Grid& grid, const Coord& start, bool part2);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
