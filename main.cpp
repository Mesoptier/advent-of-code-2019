#include <fstream>
#include <iostream>

#include "problems/day12/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day12.txt");
    std::cout << Day12::solve2(input) << std::endl;
    return 0;
}

