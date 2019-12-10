#include <fstream>
#include <iostream>

#include "problems/day10/solve.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day10.txt");
    std::cout << Day10::solve1(input) << std::endl;
    return 0;
}

