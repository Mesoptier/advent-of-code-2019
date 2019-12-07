#include <iostream>
#include <algorithm>
#include "day7.h"

int Day7::solve1(std::istream& input) {
    const auto program = readProgram(input);

    int phases[] = {0, 1, 2, 3, 4};
    int maxSignal = 0;

    do {
        int signal = 0;
        for (int phase : phases) {
            // Make a copy of the input program
            auto curProgram = program;
            curProgram.run(phase);
            curProgram.run(signal);
            signal = curProgram.output;
        }
        maxSignal = std::max(maxSignal, signal);
    } while (std::next_permutation(phases, phases + 5));

    return maxSignal;
}

int Day7::solve2(std::istream& input) {
    const auto program = readProgram(input);

    int phases[] = {5, 6, 7, 8, 9};
    int maxSignal = 0;

    do {
        // Construct 5 copies of the program which we can modify at will
        std::vector<Program> programs(5, program);

        // Prime the programs with their phases
        for (int i = 0; i < 5; ++i) {
            auto& curProgram = programs[i];
            int phase = phases[i];
            curProgram.run(phase);
        }

        int signal = 0;

        // Feedback loop until the programs have halted
        while (!programs.front().halted) {
            for (auto& curProgram : programs) {
                curProgram.run(signal);
                signal = curProgram.output;
            }
        }

        maxSignal = std::max(maxSignal, signal);
    } while (std::next_permutation(phases, phases + 5));

    return maxSignal;
}

Day7::Program Day7::readProgram(std::istream& input) {
    std::vector<int> instructions;
    while (input) {
        std::string s;
        if (!std::getline(input, s, ',')) {
            break;
        }
        instructions.push_back(std::stoi(s));
    }
    return {instructions};
}

int Day7::Program::run(int input) {
    // Number of parameters for each instruction
    int nParams[] = {0, 3, 3, 1, 1, 2, 2, 3, 3};

    bool usedInput = false;

    while (instructions[ip] != 99) {
        int instruction = instructions[ip];

        // Get two-digit opcode
        int opcode = instruction % 100;
        instruction /= 100;

        // Get parameters
        std::vector<int> params; // Raw parameters
        std::vector<int> values; // Handled based parameter mode
        for (int i = 0; i < nParams[opcode]; ++ i) {
            int mode = instruction % 10;
            instruction /= 10;

            int param = instructions[ip + 1 + i];
            params.push_back(param);

            if (mode == 0) { // Position mode
                values.push_back(instructions[param]);
            } else { // Immediate mode
                values.push_back(param);
            }
        }

        bool ipModified = false;

        switch (opcode) {
            case 1: // add
                instructions[params[2]] = values[0] + values[1];
                break;
            case 2: // multiply
                instructions[params[2]] = values[0] * values[1];
                break;
            case 3: // input
                if (usedInput) {
                    return 1;
                }
                instructions[params[0]] = input;
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
                instructions[params[2]] = values[0] < values[1];
                break;
            case 8: // equals
                instructions[params[2]] = values[0] == values[1];
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
