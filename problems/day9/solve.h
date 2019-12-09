#pragma once

#include <istream>
#include <vector>
#include <map>

namespace Day9 {
    struct Program {
        using Int = long long int;

        std::map<Int, Int> memory;
        Int ip = 0; // Instruction pointer
        Int rb = 0; // Relative base
        Int output = 0;
        bool halted = false;

        explicit Program(std::istream& input);

        Int& at(Int index);

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
        int run(Int input);
    };

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
