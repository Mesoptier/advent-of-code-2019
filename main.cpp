#include <fstream>
#include <iostream>

#include "problems/day15/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day15.txt");
    std::cout << Day15::solve1(input) << std::endl;
    return 0;
}

