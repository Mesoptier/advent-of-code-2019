#include "solve.h"

namespace Day21 {

    long long int solve1(std::istream& input) {
        Program program(input);

        std::string script =
            // Is there a place to land?
            "OR D J\n"

            // Is there a reason to jump? (i.e. any holes between @ and D?)
            "OR A T\n"
            "AND B T\n"
            "AND C T\n"
            "NOT T T\n"
            "AND T J\n"

            // Walk!
            "WALK\n";

        program.input_push(script);
        program.run();

        long long int output;
        while (program.has_output()) {
            output = program.output_pop();
        }
        return output;
    }

    long long int solve2(std::istream& input) {
        Program program(input);

        std::string script =
            // Is there a place to go after we land?
            "OR E J\n"
            "OR H J\n"

            // Is there a place to land?
            "AND D J\n"

            // Is there a reason to jump? (i.e. any holes between @ and D?)
            "OR A T\n"
            "AND B T\n"
            "AND C T\n"
            "NOT T T\n"
            "AND T J\n"

            // Run!
            "RUN\n";

        program.input_push(script);
        program.run();

        long long int output;
        while (program.has_output()) {
            output = program.output_pop();
        }
        return output;
    }

}
