#pragma once

#include <istream>
#include <vector>

namespace Day7 {
    struct Program {
        std::vector<int> instructions;
        int ip = 0;
        int output = 0;
        bool halted = false;

        /**
         * Runs the program until it halts or encounters an input or output
         * instruction at which it returns to the caller to go deal with it.
         *
         * @param input Value to be passed to the next input instruction.
         * @return int Indicates current program state:
         *  - 0 if halted
         *  - 1 if program needs to be called with a next input
         *  - 2 if program just outputted
         */
        int run(int input);
    };

    int solve1(std::istream& input);
    int solve2(std::istream& input);

    Program readProgram(std::istream& input);
}
