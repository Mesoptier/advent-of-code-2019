#pragma once

#include <array>
#include <queue>

namespace intcode {

    template<class Int = int, class Container = std::array<Int, 512>>
    class Program
    {
    private:
        Container memory;
        Int ip = 0; // Instruction pointer
        Int rb = 0; // Relative base
        std::queue<Int> output;
        std::queue<Int> input;
        bool halted = false;

    public:
        explicit Program(std::istream& input) {
            Int index = 0;
            while (input) {
                std::string s;
                if (!std::getline(input, s, ',')) {
                    break;
                }
                memory[index++] = std::stoll(s);
            }
        }

        /**
         * Get reference to value at specified memory location.
         */
        Int& at(Int index) {
            return memory.at(index);
        }

        /**
         * Push a value on the input queue.
         */
        void input_push(Int value) {
            input.push(value);
        }

        /**
         * Peek at the next output value, without altering the output queue.
         */
        Int output_peek() const {
            return output.front();
        }

        /**
         * Get and the next value from the output queue.
         */
        Int output_pop() {
            Int value = output.front();
            output.pop();
            return value;
        }

        /**
         * Run the program until either input is needed (return true), or the
         * program is halted (return false).
         */
        bool run() {
            // Number of parameters for each instruction
            int nParams[] = {0, 3, 3, 1, 1, 2, 2, 3, 3, 1};

            while (at(ip) != 99) {
                int instruction = at(ip);

                // Get two-digit opcode
                int opcode = instruction % 100;
                instruction /= 100;

                // Get parameters
                std::vector<Int> params; // Position parameters
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
    };

}