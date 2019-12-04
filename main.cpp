#include <fstream>
#include <iostream>

#include "problems/day4/day4.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day4.txt");
    std::cout << Day4::solve1(input) << std::endl;
    return 0;
}

