#include <iostream>
#include <cassert>
#include "solve.h"

using namespace intcode;

namespace Day19 {

    int solve1(std::istream& input) {
        Program<> program(input);
        program.run(); // Run program until first input is requested

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
        Program<> program(input);
        program.run(); // Run program until first input is requested

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

}
