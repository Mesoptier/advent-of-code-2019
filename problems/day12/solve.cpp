#include "solve.h"
#include <regex>
#include <iostream>
#include <numeric>
#include <set>

namespace Day12 {

    int solve1(std::istream& input) {
        auto state = readInput(input);

        for (int step = 0; step < 1000; ++step) {
            simulate(state);
        }

        int totalEnergy = 0;
        for (const auto& moon : state) {
            totalEnergy += moon.pot() * moon.kin();
        }
        return totalEnergy;
    }

    long long int solve2(std::istream& input) {
        auto state = readInput(input);

        bool foundPeriod[3] = { false, false, false };
        long long int period[3] = { 0, 0, 0 };

        while (!foundPeriod[0] || !foundPeriod[1] || !foundPeriod[2]) {
            simulate(state);

            // Check periodicity
            bool isInitial[3] = { true, true, true };

            for (auto& moon : state) {
                for (int dim = 0; dim < 3; ++dim) {
                    if (moon.pos[dim] != moon.ipos[dim] || moon.vel[dim] != 0) {
                        isInitial[dim] = false;
                    }
                }
            }

            for (int dim = 0; dim < 3; ++dim) {
                if (!foundPeriod[dim]) {
                    period[dim] += 1;

                    if (isInitial[dim]) {
                        foundPeriod[dim] = true;
                    }
                }
            }
        }

        return std::lcm(period[0], std::lcm(period[1], period[2]));
    }

    std::vector<Moon> readInput(std::istream& input) {
        std::vector<Moon> state {};

        std::regex e("<x=(.+), y=(.+), z=(.+)>");
        for (std::string line; std::getline(input, line);) {
            std::smatch cm;
            std::regex_match(line, cm, e);

            state.emplace_back(
                stoi(cm[1]),
                stoi(cm[2]),
                stoi(cm[3])
            );
        }

        return state;
    }

    void simulate(State& state) {
        for (auto& moon1 : state) {
            for (auto& moon2 : state) {
                for (int dim = 0; dim < 3; ++dim) {
                    if (moon1.pos[dim] < moon2.pos[dim]) {
                        moon1.vel[dim] += 1;
                        moon2.vel[dim] -= 1;
                    }
                }
            }
        }

        for (auto& moon : state) {
            moon.pos += moon.vel;
        }
    }

}
