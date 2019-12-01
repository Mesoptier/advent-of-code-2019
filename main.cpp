#include <fstream>
#include <iostream>

#include "problems/day1/day1.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day1.txt");
    std::cout << Day1::solve2(input) << std::endl;
    return 0;
}

