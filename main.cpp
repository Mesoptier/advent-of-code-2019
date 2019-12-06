#include <fstream>
#include <iostream>

#include "problems/day6/day6.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day6.txt");
    std::cout << Day6::solve2(input) << std::endl;
    return 0;
}

