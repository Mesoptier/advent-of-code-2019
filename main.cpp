#include <fstream>
#include <iostream>

#include "problems/day5/day5.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day5.txt");
    std::cout << Day5::solve1(input) << std::endl;
    return 0;
}

