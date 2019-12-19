#include <iostream>
#include <cassert>
#include "solve.h"

using namespace intcode;

namespace Day19 {

    int solve1(std::istream& input) {
        const Program<> program(input);
        Grid<char> grid;

        int sum = 0;

        for (int x = 0; x < 50; ++x) {
            for (int y = 0; y < 50; ++y) {
                auto state = get_state(program, {x, y});
                sum += state;
                grid[{x, y}] = state ? '#' : ' ';
            }
        }

        return sum;
    }

    int solve2(std::istream& input) {
        const Program<> program(input);

        // There are some empty spaces before this point
        const int start_x = 6;
        const int start_y = 5;

        int y = start_y;
        for (int x = start_x + 1; x < 9999; ++x) {
            // Find the greatest Y in the beam at the current X position
            while (!get_state(program, {x, y})) {
                ++y;
            }

            // Find first position where a 100x100 square could fit in the beam
            if (x > 99 && get_state(program, {x - 99, y + 99})) {
                return (x - 99) * 10000 + y;
            }
        }

        return -2;
    }

    bool get_state(const Program<>& program, Coord pos) {
        auto clone = program;
        clone.input_push(pos.x);
        clone.input_push(pos.y);
        clone.run();
        return clone.output_peek();
    }

    void print_grid(const Grid<char>& grid) {
        int minX = INT_MAX;
        int maxX = 0;
        int minY = INT_MAX;
        int maxY = 0;

        for (const auto& kv : grid) {
            minX = std::min(kv.first.x, minX);
            maxX = std::max(kv.first.x, maxX);
            minY = std::min(kv.first.y, minY);
            maxY = std::max(kv.first.y, maxY);
        }

        for (int y = minY; y <= maxY; ++y) {
            for (int x = minX; x <= maxX; ++x) {
                auto it = grid.find({x, y});

                if (it == grid.end()) {
                    std::cout << '.';
                    continue;
                }

                std::cout << it->second;
            }
            std::cout << '\n';
        }
    }
}
