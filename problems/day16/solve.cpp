#include <iostream>
#include <cmath>
#include "solve.h"

namespace Day16 {

    int solve1(std::istream& input) {
        std::vector<int> signal = read_signal(input);
        std::vector<int> next_signal(signal.size());

        for (int phase = 0; phase < 100; ++phase) {
            for (int out_pos = 0; out_pos < signal.size(); ++out_pos) {
                next_signal[out_pos] = 0;

                for (int in_pos = out_pos; in_pos < signal.size(); ++in_pos) {
                    int pattern_value = (in_pos + 1) / (out_pos + 1) % 4;
                    if (pattern_value > 1) {
                        pattern_value = -(pattern_value - 2);
                    }

                    if (pattern_value == 0) {
                        in_pos += out_pos;
                        continue;
                    }

                    next_signal[out_pos] += signal[in_pos] * pattern_value;
                }

                next_signal[out_pos] = abs(next_signal[out_pos]) % 10;
            }

            signal.swap(next_signal);
        }

        return get_digits(signal, 0, 8);
    }

    int solve2(std::istream& input) {
        std::vector<int> input_signal = read_signal(input);
        int input_size = input_signal.size();

        // Get offset to relevant part of input signal
        int offset = get_digits(input_signal, 0, 7);

        // Extract the relevant part of the signal
        std::vector<int> signal;
        for (int i = offset; i < input_size * 10000; ++i) {
            signal.push_back(input_signal[i % input_size]);
        }
        int signal_size = signal.size();
        std::vector<int> next_signal(signal_size);

        for (int phase = 0; phase < 100; ++phase) {

            // Pattern values form groups of size (offset + out_pos + 1) which
            // happens to always be greater than the signal size. So for our
            // part of the signal the pattern value is always 1. So the value
            // for each output digit is the sum of all input digits with equal
            // or greater position (that is, in_pos >= out_pos). To simplify
            // this computation, I keep a sum (modulo 10) of all input digits
            // to the right (in_pos > out_pos) which only needs to be updated
            // for the input digit at the current position (in_pos == out_pos).

            int value = 0;
            for (int out_pos = signal_size - 1; out_pos >= 0; --out_pos) {
                value += signal[out_pos];
                value %= 10;
                next_signal[out_pos] = value;
            }

            signal.swap(next_signal);
        }

        return get_digits(signal, 0, 8);
    }

    std::vector<int> read_signal(std::istream& input) {
        std::vector<int> signal;
        while (input.peek() != EOF) {
            signal.push_back(input.get() - 48);
        }
        return signal;
    }

    int get_digits(const std::vector<int>& signal, int offset, int length) {
        int result = 0;
        for (int i = 0; i < length; ++i) {
            result += signal[offset + i] * pow(10, length - i - 1);
        }
        return result;
    }
}
