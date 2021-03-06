#include <fstream>
#include <iostream>
#include <chrono>

#include "problems/day25/solve.h"

#define TIMING_START auto __timing_start = std::chrono::high_resolution_clock::now();
#define TIMING_FINISH \
    auto __timing_finish = std::chrono::high_resolution_clock::now(); \
    std::chrono::duration<double> __timing_elapsed = __timing_finish - __timing_start; \
    std::cout << "Elapsed time: " << __timing_elapsed.count() << " s\n";

int main() {
    std::ifstream input;
    input.open("../inputs/day25.txt");
    TIMING_START
    std::cout << "Part 1: " << Day25::solve1(input) << std::endl;
//    std::cout << "Part 2: " << Day25::solve2(input) << std::endl;
    TIMING_FINISH
    return 0;
}

