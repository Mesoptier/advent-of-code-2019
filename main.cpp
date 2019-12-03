#include <fstream>
#include <iostream>

#include "problems/day3/day3.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day3.txt");
    std::cout << Day3::solve2(input) << std::endl;
    return 0;
}

