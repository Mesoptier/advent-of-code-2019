#include <iostream>
#include <cassert>
#include "solve.h"

namespace Day17 {

    int solve1(std::istream& input) {
        Program program(input);
        Grid scaffolds;
        Coord robot;

        {
            int x = 0;
            int y = 0;

            program.run();
            while (!program.output.empty()) {
                switch (program.output.front()) {
                    case 35:
                        scaffolds[{x, y}] = 1;
                        x++;
                        break;
                    case 46:
//                        scaffolds[{x, y}] = 0;
                        x++;
                        break;
                    case 10:
                        y++;
                        x = 0;
                        break;
                    default:
                        std::cout << program.output.front() << std::endl;
                        robot = {x, y};
                        scaffolds[{x, y}] = 1;
                        x++;
                        break;
                }

                program.output.pop();
            }
        }

        int sum = 0;

        for (auto kv : scaffolds) {
            const auto coord = kv.first;
            if (scaffolds.count(coord.step(North)) && scaffolds.count(coord.step(East)) && scaffolds.count(coord.step(South)) && scaffolds.count(coord.step(West))) {
                sum += coord.x * coord.y;
            }
        }

        return sum;
    }

    int solve2(std::istream& input) {
        // A,B,A,B,C,A,B,C,A,C
        // A = R,6,L,10,R,8
        // B = R,8,R,12,L,8,L,8
        // C = L,10,R,6,R,6,L,8

        const std::string solution =
            "A,B,A,B,C,A,B,C,A,C\n"
            "R,6,L,10,R,8\n"
            "R,8,R,12,L,8,L,8\n"
            "L,10,R,6,R,6,L,8\n"
            "n\n";

        Program program(input);
        program.at(0) = 2;

        for (int c : solution) {
            program.input.push(c);
        }

        program.run();

        while (program.output.size() > 1) {
            char c = program.output.front();
            std::cout << c;
            program.output.pop();
        }

        return program.output.front();
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
                    case 1:
                        std::cout << '#';
                        break;
                    case 0:
                        std::cout << ' ';
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
