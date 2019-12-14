#include <fstream>
#include <iostream>

#include "problems/day14/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day14.txt");
    std::cout << Day14::solve2(input) << std::endl;
    return 0;
}

