#include <iostream>
#include "solve.h"

int Day11::solve1(std::istream& input) {
    auto grid = runRobot(input, 0);
    return grid.size();
}

int Day11::solve2(std::istream& input) {
    auto grid = runRobot(input, 1);
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
            std::cout << (grid[{x, y}] ? '#' : ' ');
        }
        std::cout << '\n';
    }

    return -1;
}

Day11::Direction Day11::turnLeft(Day11::Direction direction) {
    switch (direction) {
        case Up:
            return Left;
        case Left:
            return Down;
        case Down:
            return Right;
        case Right:
            return Up;
    }
}

Day11::Direction Day11::turnRight(Day11::Direction direction)  {
    switch (direction) {
        case Up:
            return Right;
        case Left:
            return Up;
        case Down:
            return Left;
        case Right:
            return Down;
    }
}

Day11::Grid Day11::runRobot(std::istream& input, int startValue) {
    Program program(input);
    std::map<Coord, int> grid;
    Coord position{0, 0};
    Direction direction = Up;

    program.input.push(startValue);

    while (!program.halted || !program.output.empty()) {
        if (!program.halted) {
            program.run();
        }

        int paint = program.output.front();
        program.output.pop();
        int turn = program.output.front();
        program.output.pop();

        grid[position] = paint;
        direction = turn ? turnRight(direction) : turnLeft(direction);

        switch (direction) {
            case Up:
                position.y -= 1;
                break;
            case Left:
                position.x -= 1;
                break;
            case Right:
                position.x += 1;
                break;
            case Down:
                position.y += 1;
                break;
        }

        program.input.push(grid[position]);
    }

    return grid;
}

Day11::Program::Program(std::istream& input) {
    Int index = 0;
    while (input) {
        std::string s;
        if (!std::getline(input, s, ',')) {
            break;
        }
        memory.insert({index++, std::stoll(s)});
    }
}

Day11::Program::Int& Day11::Program::at(Int index) {
    auto it = memory.find(index);
    if (it == memory.end()) {
        return memory.insert({index, 0}).first->second;
    }
    return it->second;
}

bool Day11::Program::run() {
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