#include <fstream>
#include <iostream>

#include "problems/day2/day2.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day2.txt");
    std::cout << Day2::solve2(input) << std::endl;
    return 0;
}

