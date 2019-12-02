#include <iostream>
#include "day2.h"

int Day2::solve1(std::istream& input) {
    auto program = readProgram(input);
    program[1] = 12;
    program[2] = 2;
    runProgram(program);

    return program[0];
}

int Day2::solve2(std::istream& input) {
    const auto program = readProgram(input);
    for (int noun = 0; noun < 99; ++noun) {
        for (int verb = 0; verb < 99; ++verb) {
            auto testProgram = program;
            testProgram[1] = noun;
            testProgram[2] = verb;
            runProgram(testProgram);

            if (testProgram[0] == 19690720) {
                return 100 * noun + verb;
            }
        }
    }
    return 0;
}

std::vector<int> Day2::readProgram(std::istream& input) {
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

void Day2::runProgram(std::vector<int>& program) {
    int ip = 0; // Instruction pointer
    while (program[ip] != 99) {
        int opcode = program[ip];
        int arg1 = program[ip + 1];
        int arg2 = program[ip + 2];
        int arg3 = program[ip + 3];

        switch (opcode) {
            case 1:
                program[arg3] = program[arg1] + program[arg2];
                break;
            case 2:
                program[arg3] = program[arg1] * program[arg2];
                break;
            default:
                throw std::logic_error("Unknown opcode");
        }

        ip += 4;
    }
}

