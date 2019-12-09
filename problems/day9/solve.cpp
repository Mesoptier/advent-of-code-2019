#include <iostream>
#include "solve.h"

int Day9::solve1(std::istream& input) {
    Program program(input);
    while (!program.halted) {
        if (program.run(1) == 2) {
            std::cout << program.output << '\n';
        }
    }
    return 0;
}

int Day9::solve2(std::istream& input) {
    Program program(input);
    while (!program.halted) {
        if (program.run(2) == 2) {
            std::cout << program.output << '\n';
        }
    }
    return 0;
}

Day9::Program::Program(std::istream& input) {
    Int index = 0;
    while (input) {
        std::string s;
        if (!std::getline(input, s, ',')) {
            break;
        }
        memory.insert({index++,std::stoll(s)});
    }
}

Day9::Program::Int& Day9::Program::at(Int index) {
    auto it = memory.find(index);
    if (it == memory.end()) {
        return memory.insert({index, 0}).first->second;
    }
    return it->second;
}

int Day9::Program::run(Int input) {
    // Number of parameters for each instruction
    int nParams[] = {0, 3, 3, 1, 1, 2, 2, 3, 3, 1};

    bool usedInput = false;

    while (at(ip) != 99) {
        int instruction = at(ip);

        // Get two-digit opcode
        int opcode = instruction % 100;
        instruction /= 100;

        // Get parameters
        std::vector<Int> params; // Raw parameters
        std::vector<Int> values; // Handled based parameter mode
        for (int i = 0; i < nParams[opcode]; ++ i) {
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
                if (usedInput) {
                    return 1;
                }
                at(params[0]) = input;
                usedInput = true;
                break;
            case 4: // output
                output = values[0];
                ip += nParams[opcode] + 1;
                return 2;
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
    return 0;
}