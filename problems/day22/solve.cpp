#include <vector>
#include <tuple>
#include <iostream>
#include "solve.h"
#include "mod_int.h"

namespace Day22 {

    // Mathematics used:
    // - https://en.wikipedia.org/wiki/Modular_arithmetic
    // - https://en.wikipedia.org/wiki/Modular_multiplicative_inverse
    // - https://en.wikipedia.org/wiki/Geometric_series#Formula

    template<class int_t, int_t total>
    std::pair<mod_int<int_t, total>, mod_int<int_t, total>> parse_input(std::istream& input) {
        mod_int<int_t, total> a = 1;
        mod_int<int_t, total> b = 0;
        for (std::string line; std::getline(input, line);) {
            mod_int<int_t, total> am = 1;
            mod_int<int_t, total> bm = 0;

            if (line.rfind("deal into new stack", 0) == 0) {
                am = total - 1;
                bm = total - 1;
            } else if (line.rfind("cut", 0) == 0) {
                auto cut = std::stoi(line.substr(4));
                am = 1;
                bm = (total - cut) % total;
            } else if (line.rfind("deal with increment", 0) == 0) {
                auto increment = std::stoi(line.substr(20));
                am = increment;
                bm = 0;
            }

            a = a * am;
            b = b * am + bm;
        }
        return {a, b};
    }

    unsigned int solve1(std::istream& input) {
        using int_t = unsigned int;
        constexpr int_t total = 10007; // Prime
        using card_t = mod_int<int_t, total>;

        card_t start_pos = 2019;

        card_t a, b;
        std::tie(a, b) = parse_input<int_t, total>(input);

        auto final_pos = start_pos * a + b;

        return (int_t) final_pos;
    }

    unsigned long long int solve2(std::istream& input) {
        using int_t = unsigned long long int;
        constexpr int_t total = 119315717514047; // Prime
        using card_t = mod_int<int_t, total>;

        int_t n = 101741582076661; // Number of shuffles
        card_t final_pos = 2020;

        card_t a, b;
        std::tie(a, b) = parse_input<int_t, total>(input);

        // Inverse (works since total is prime)
        card_t ai = pow(a, total - 2);
        card_t bi = total - (int_t) b;

        auto start_pos = final_pos * pow(ai, n)
            // next line equals + (bi * ai^1) + (bi * ai^2) + ... + (bi * ai^n)
            + ai * bi * ((pow(ai, n) - 1) * pow(ai - 1, total - 2));

        return (int_t) start_pos;
    }

}
