#include <fstream>
#include <iostream>

#include "problems/day13/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day13.txt");
    std::cout << Day13::solve2(input) << std::endl;
    return 0;
}

