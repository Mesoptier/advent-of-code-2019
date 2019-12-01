#include <iostream>
#include "day1.h"

int Day1::solve1(std::istream& input) {
    int mass;
    int totalMass = 0;
    while (input >> mass) {
        totalMass += mass / 3 - 2;
    }
    return totalMass;
}

int Day1::solve2(std::istream& input) {
    int mass;
    int fuel;
    int totalFuel = 0;

    while (input >> mass) {
        fuel = mass / 3 - 2;
        while (fuel > 0) {
            totalFuel += fuel;
            fuel = fuel / 3 - 2;
        }
    }

    return totalFuel;
}

