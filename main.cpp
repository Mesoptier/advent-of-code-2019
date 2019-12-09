#include <fstream>
#include <iostream>

#include "problems/day9/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day9.txt");
    std::cout << Day9::solve2(input) << std::endl;
    return 0;
}

