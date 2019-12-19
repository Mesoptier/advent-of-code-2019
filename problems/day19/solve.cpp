#include <iostream>
#include <cassert>
#include "solve.h"

namespace Day19 {

    int solve1(std::istream& input) {
        const Program program(input);
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
        const Program program(input);

        // There are some empty spaces before this point
        int start_x = 6;
        int start_y = 5;

        std::vector<int> max_y(start_x);
        std::vector<int> min_y(start_x);

        max_y.push_back(start_y);
        min_y.push_back(start_y);

        for (int x = start_x + 1; x < 9999; ++x) {
            int y;

            // Find max Y
            y = max_y[x - 1];
            while (!get_state(program, {x, y})) {
                ++y;
            }
            max_y.push_back(y);

            // Find min Y
            y = min_y[x - 1];
            while (!get_state(program, {x, y})) {
                ++y;
            }
            while (get_state(program, {x, y})) {
                ++y;
            }
            min_y.push_back(y - 1);

            // Find first position where a 100x100 square could fit in the beam
            if (x > start_x + 100) {
                if (min_y[x - 99] - max_y[x] == 99) {
                    return (x - 99) * 10000 + max_y[x];
                }
            }
        }

        return -2;
    }

    bool get_state(const Program& program, Coord pos) {
        Program clone = program;
        clone.input.push(pos.x);
        clone.input.push(pos.y);
        clone.run();
        return clone.output.front();
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

    Program::Program(std::istream& input) {
        Int index = 0;
        while (input) {
            std::string s;
            if (!std::getline(input, s, ',')) {
                break;
            }
            memory.insert({index++, std::stoll(s)});
        }
    }

    Program::Int& Program::at(Int index) {
        auto it = memory.find(index);
        if (it == memory.end()) {
            return memory.insert({index, 0}).first->second;
        }
        return it->second;
    }

    bool Program::run() {
        // Number of parameters for each instruction
        int nParams[] = {0, 3, 3, 1, 1, 2, 2, 3, 3, 1};

        while (at(ip) != 99) {
            int instruction = at(ip);

            // Get two-digit opcode
            int opcode = instruction % 100;
            instruction /= 100;

            // Get parameters
            std::vector<Int> params; // Raw parameters
            std::vector<Int> values; // Handled based parameter mode
            for (int i = 0; i < nParams[opcode]; ++i) {
                int mode = instruction % 10;
                instruction /= 10;

                Int param = at(ip + 1 + i);

                switch (mode) {
                    case 0: // Position mode
                        params.push_back(param);
                        values.push_back(at(param));
                        break;
                    case 1: // Immediate mode
                        params.push_back(param);
                        values.push_back(param);
                        break;
                    case 2: // Relative mode
                        params.push_back(rb + param);
                        values.push_back(at(rb + param));
                        break;
                }
            }

            bool ipModified = false;

            switch (opcode) {
                case 1: // add
                    at(params[2]) = values[0] + values[1];
                    break;
                case 2: // multiply
                    at(params[2]) = values[0] * values[1];
                    break;
                case 3: // input
                    if (input.empty()) {
                        return true;
                    }
                    at(params[0]) = input.front();
                    input.pop();
                    break;
                case 4: // output
                    output.push(values[0]);
                    break;
                case 5: // jump-if-true
                    if (values[0] != 0) {
                        ip = values[1];
                        ipModified = true;
                    }
                    break;
                case 6: // jump-if-false
                    if (values[0] == 0) {
                        ip = values[1];
                        ipModified = true;
                    }
                    break;
                case 7: // less than
                    at(params[2]) = values[0] < values[1];
                    break;
                case 8: // equals
                    at(params[2]) = values[0] == values[1];
                    break;
                case 9: // adjust relative base
                    rb += values[0];
                    break;
                default:
                    throw std::logic_error("Unknown opcode");
            }

            if (!ipModified) {
                ip += nParams[opcode] + 1;
            }
        }

        halted = true;
        return false;
    }

}