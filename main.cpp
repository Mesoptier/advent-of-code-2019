#include <fstream>
#include <iostream>

#include "problems/day7/day7.h"

int main() {
    std::ifstream input;
    input.open("../inputs/day7.txt");
    std::cout << Day7::solve2(input) << std::endl;
    return 0;
}

