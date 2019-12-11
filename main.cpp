#include <fstream>
#include <iostream>

#include "problems/day11/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day11.txt");
    std::cout << Day11::solve2(input) << std::endl;
    return 0;
}

