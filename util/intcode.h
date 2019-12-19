#pragma once

#include <array>
#include <queue>
#include <cmath>

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

        inline int param_mode(int n) {
            return (at(ip) / pow10(n + 2)) % 10;
        }

        Int param_pos(int n) {
            switch (param_mode(n)) {
                case 0: // Position mode
                case 1: // Immediate mode
                    return at(ip + 1 + n);
                case 2: // Relative mode
                    return at(ip + 1 + n) + rb;
            }
        }

        Int param_val(int n) {
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