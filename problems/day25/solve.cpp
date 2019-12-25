#include "solve.h"

namespace Day25 {

    int solve1(std::istream& input) {
        Program program(input);

        while (!program.halted()) {
            if (program.run()) {
                program.print_output();

                std::string line;
                std::getline(std::cin, line);
                program.push_input(line);
                program.push_input('\n');
            } else {
                program.print_output();
            }
        }

        return 0;
    }

    int solve2(std::istream& input) {
        return 0;
    }

}
