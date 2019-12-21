#pragma once

#include <array>
#include <queue>
#include <cmath>
#include <iostream>

namespace {
    int pow10(int n) {
        if (n == 0) return 1;
        if (n == 1) return 10;
        if (n == 2) return 100;
        if (n == 3) return 1000;
        if (n == 4) return 10000;
        if (n == 5) return 100000;
        throw std::invalid_argument("pow10 only supports 0 <= n <= 5");
    }
}

namespace intcode {

    template<class int_t, class memory_t>
    class Program
    {
    private:
        memory_t memory;
        int_t ip = 0; // Instruction pointer
        int_t rb = 0; // Relative base
        std::queue<int_t> output;
        std::queue<int_t> input;
        bool halted = false;

        inline int param_mode(int n) const {
            return (at(ip) / pow10(n + 2)) % 10;
        }

        int_t param_pos(int n) const {
            switch (param_mode(n)) {
                case 0: // Position mode
                case 1: // Immediate mode
                    return at(ip + 1 + n);
                case 2: // Relative mode
                    return at(ip + 1 + n) + rb;
            }
        }

        int_t param_val(int n) const {
            switch (param_mode(n)) {
                case 0: // Position mode
                    return at(at(ip + 1 + n));
                case 1: // Immediate mode
                    return at(ip + 1 + n);
                case 2: // Relative mode
                    return at(at(ip + 1 + n) + rb);
            }
        }

    public:
        explicit Program(std::istream& input) {
            int_t index = 0;
            while (input) {
                std::string s;
                if (!std::getline(input, s, ',')) {
                    break;
                }
                memory.at(index++) = std::stoll(s);
            }
        }

        /**
         * Get reference to value at specified memory location.
         */
        int_t& at(int_t index) {
            return memory.at(index);
        }
        const int_t at(int_t index) const {
            return memory.at(index);
        }

        /**
         * Push a value on the input queue.
         */
        void push_input(int_t value) {
            input.push(value);
        }

        /**
         * Push each char in the provided string onto the input queue.
         */
        void push_input(const std::string& value) {
            for (char c : value) {
                push_input(c);
            }
        }

        /**
         * Whether the output queue has values in it.
         */
        bool has_output() const {
            return !output.empty();
        }

        /**
         * Peek at the next output value, without altering the output queue.
         */
        int_t peek_output() const {
            return output.front();
        }

        /**
         * Get and the next value from the output queue.
         */
        int_t pop_output() {
            int_t value = output.front();
            output.pop();
            return value;
        }

        /**
         * Prints the entire output queue to stdout.
         */
        void print_output() {
            while (!output.empty()) {
                std::cout << (char) pop_output();
            }
        }

        /**
         * Run the program until either input is needed (return true), or the
         * program is halted (return false).
         */
        bool run() {
            while (at(ip) != 99) {
                // Get two-digit opcode
                int opcode = at(ip) % 100;

                switch (opcode) {
                    case 1: // add
                        at(param_pos(2)) = param_val(0) + param_val(1);
                        ip += 4;
                        break;
                    case 2: // multiply
                        at(param_pos(2)) = param_val(0) * param_val(1);
                        ip += 4;
                        break;
                    case 3: // input
                        if (input.empty()) {
                            return true;
                        }
                        at(param_pos(0)) = input.front();
                        input.pop();
                        ip += 2;
                        break;
                    case 4: // output
                        output.push(param_val(0));
                        ip += 2;
                        break;
                    case 5: // jump-if-true
                        if (param_val(0) != 0) {
                            ip = param_val(1);
                        } else {
                            ip += 3;
                        }
                        break;
                    case 6: // jump-if-false
                        if (param_val(0) == 0) {
                            ip = param_val(1);
                        } else {
                            ip += 3;
                        }
                        break;
                    case 7: // less than
                        at(param_pos(2)) = param_val(0) < param_val(1);
                        ip += 4;
                        break;
                    case 8: // equals
                        at(param_pos(2)) = param_val(0) == param_val(1);
                        ip += 4;
                        break;
                    case 9: // adjust relative base
                        rb += param_val(0);
                        ip += 2;
                        break;
                    default:
                        throw std::logic_error("Unknown opcode");
                }
            }

            halted = true;
            return false;
        }
    };

}