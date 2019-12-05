#include <iostream>
#include "day5.h"

int Day5::solve1(std::istream& input) {
    auto program = readProgram(input);
    return runProgram(program, 1);
}

int Day5::solve2(std::istream& input) {
    auto program = readProgram(input);
    return runProgram(program, 5);
}

std::vector<int> Day5::readProgram(std::istream& input) {
    std::vector<int> program;
    while (input) {
        std::string s;
        if (!std::getline(input, s, ',')) {
            break;
        }
        program.push_back(std::stoi(s));
    }
    return program;
}

int Day5::runProgram(std::vector<int>& program, int input) {
    // Number of parameters for each instruction
    int nParams[] = {0, 3, 3, 1, 1, 2, 2, 3, 3};

    int output = -1;

    int ip = 0; // Instruction pointer
    while (program[ip] != 99) {
        int instruction = program[ip];

        // Get two-digit opcode
        int opcode = instruction % 100;
        instruction /= 100;

        // Get parameters
        std::vector<int> params; // Raw parameters
        std::vector<int> values; // Handled based parameter mode
        for (int i = 0; i < nParams[opcode]; ++ i) {
            int mode = instruction % 10;
            instruction /= 10;

            int param = program[ip + 1 + i];
            params.push_back(param);

            if (mode == 0) { // Position mode
                values.push_back(program[param]);
            } else { // Immediate mode
                values.push_back(param);
            }
        }

        bool ipModified = false;

        switch (opcode) {
            case 1: // add
                program[params[2]] = values[0] + values[1];
                break;
            case 2: // multiply
                program[params[2]] = values[0] * values[1];
                break;
            case 3: // input
                program[params[0]] = input;
                break;
            case 4: // output
                output = values[0];
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
                program[params[2]] = values[0] < values[1];
                break;
            case 8: // equals
                program[params[2]] = values[0] == values[1];
                break;
            default:
                throw std::logic_error("Unknown opcode");
        }

        if (!ipModified) {
            ip += nParams[opcode] + 1;
        }
    }

    return output;
}

