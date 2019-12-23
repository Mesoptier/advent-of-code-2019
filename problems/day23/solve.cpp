#include "solve.h"

namespace Day23 {

    int_t solve1(std::istream& input) {
        Program base_program(input);
        std::vector<Program> computers;

        for (int address = 0; address < 50; ++address) {
            computers.emplace_back(base_program);
            computers.back().push_input(address);
        }

        while (true) {
            for (auto& c : computers) {
                if (c.run()) {
                    c.push_input(-1);
                }

                while (c.has_output()) {
                    int address = c.pop_output();
                    int_t x = c.pop_output();
                    int_t y = c.pop_output();

                    if (address == 255) {
                        return y;
                    } else {
                        computers[address].push_input(x);
                        computers[address].push_input(y);
                    }
                }
            }
        }
    }

    int solve2(std::istream& input) {
        Program base_program(input);
        std::vector<Program> computers;

        int_t nat_x = 0;
        int_t nat_y = 0;

        int_t prev_y = -1;

        for (int address = 0; address < 50; ++address) {
            computers.emplace_back(base_program);
            computers.back().push_input(address);
        }

        while (true) {
            bool idle = true;

            for (auto& c : computers) {
                if (c.run()) { // empty input queue
                    idle = idle && !c.has_output(); // not sending packets
                }
            }

            if (idle) {
                if (prev_y == nat_y) {
                    return nat_y;
                }
                prev_y = nat_y;

                computers[0].push_input(nat_x);
                computers[0].push_input(nat_y);
                computers[0].run();
            } else {
                prev_y = -1;
            }

            for (auto& c : computers) {
                if (c.run()) {
                    c.push_input(-1);
                }

                while (c.has_output()) {
                    int address = c.pop_output();
                    int_t x = c.pop_output();
                    int_t y = c.pop_output();

                    if (address == 255) {
                        nat_x = x;
                        nat_y = y;
                    } else {
                        computers[address].push_input(x);
                        computers[address].push_input(y);
                    }
                }
            }
        }
    }

}
