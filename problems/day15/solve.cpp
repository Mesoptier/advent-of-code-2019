#include <iostream>
#include <cassert>
#include "solve.h"

namespace Day15 {

    int solve1(std::istream& input) {
        Program program(input);

        const Coord origin{0, 0};
        Grid grid; // 0 - unexplored, 1 - wall, 2 - open, 3 - goal
        flood_fill(program, grid, origin);

        return compute_cost(grid, origin, false);
    }

    int solve2(std::istream& input) {
        Program program(input);

        const Coord origin{0, 0};
        Grid grid; // 0 - unexplored, 1 - wall, 2 - open, 3 - goal
        flood_fill(program, grid, origin);

        // Find goal coord
        Coord goal;
        for (auto kv : grid) {
            if (kv.second == 3) {
                goal = kv.first;
            }
        }

        return compute_cost(grid, goal, true);
    }

    int compute_cost(const Grid& grid, const Coord& start, bool part2) {
        Grid dist_to_start;
        std::queue<Coord> queue;
        queue.push(start);
        dist_to_start[start] = 0;

        while (!queue.empty()) {
            const auto current = queue.front();
            queue.pop();

            // Part 1: return distance from start to goal
            if (!part2 && grid.at(current) == 3) {
                return dist_to_start[current];
            }

            if (grid.at(current) == 1) {
                continue;
            }

            for (int i = 1; i <= 4; ++i) {
                auto dir = static_cast<Direction>(i);
                Coord neighbor = current.step(dir);

                auto dist = dist_to_start[current] + 1;
                auto it = dist_to_start.find(neighbor);
                if (it == dist_to_start.end() || dist < it->second) {
                    dist_to_start[neighbor] = dist;
                    queue.push(neighbor);
                }
            }
        }

        // Part 2:
        if (part2) {
            int max_dist = 0;
            for (auto kv : dist_to_start) {
                if (grid.at(kv.first) == 2) { // Only consider open cells
                    max_dist = std::max(max_dist, kv.second);
                }
            }
            return max_dist;
        }

        return -1;
    }

    void flood_fill(Program& program, Grid& grid, const Coord& current) {
        for (int i = 1; i <= 4; ++i) {
            auto dir = static_cast<Direction>(i);
            Coord neighbor = current.step(dir);

            if (grid[neighbor] != 0) {
                continue;
            }

            // Step forward
            program.input.push(dir);
            program.run();

            // Handle status
            int status = program.output.front();
            program.output.pop();

            switch (status) {
                case 0: // wall
                    grid[neighbor] = 1;
                    break;
                case 1: // open
                    grid[neighbor] = 2;
                    break;
                case 2: // goal
                    grid[neighbor] = 3;
                    break;
                default:
                    break;
            }

            if (status != 0) {
                flood_fill(program, grid, neighbor);

                // Step back
                program.input.push(reverse(dir));
                program.run();

                program.output.pop();
            }
        }

    }

    Direction reverse(Direction direction) {
        switch (direction) {
            case North:
                return South;
            case South:
                return North;
            case West:
                return East;
            case East:
                return West;
        }
    }

    void print_grid(const Grid& grid) {
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

                switch (it->second) {
                    case 0:
                        break;
                    case 1:
                        std::cout << '#';
                        break;
                    case 2:
                        std::cout << ' ';
                        break;
                    case 3:
                        std::cout << 'G';
                        break;
                }
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
