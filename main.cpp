#include <fstream>
#include <iostream>

#include "problems/day8/day8.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day8.txt");
    std::cout << Day8::solve2(input) << std::endl;
    return 0;
}

