#include <iostream>
#include <sstream>
#include <cmath>
#include "day3.h"

int Day3::solve1(std::istream& input) {
    return solveBoth(input, false);
}

int Day3::solve2(std::istream& input) {
    return solveBoth(input, true);
}

Day3::Wire Day3::readWire(std::istream& input) {
    std::string line;
    std::getline(input, line);
    std::stringstream ss(line);

    Wire wire;
    wire.emplace_back(0, 0);

    while (getline(ss, line, ',')) {
        int magnitude = std::stoi(line.substr(1));
        Point point = wire.back();

        switch (line[0]) {
            case 'R':
                point.first += magnitude;
                break;
            case 'L':
                point.first -= magnitude;
                break;
            case 'U':
                point.second += magnitude;
                break;
            case 'D':
                point.second -= magnitude;
                break;
        }

        wire.push_back(point);
    }

    return wire;
}

int Day3::solveBoth(std::istream& input, bool part2) {
    Wire wire1;
    Wire wire2;

    wire1 = readWire(input);
    wire2 = readWire(input);

    int minDist = INT_MAX;

    // Only needed for part 2
    int steps1 = 0;
    int steps2 = 0;

    for (int i = 1; i < wire1.size(); ++i) {
        Point a1 = wire1[i - 1];
        Point a2 = wire1[i];

        for (int j = 1; j < wire2.size(); ++j) {
            Point b1 = wire2[j - 1];
            Point b2 = wire2[j];

            if (((a1.first <= b1.first && b1.first <= a2.first) || (a1.first >= b1.first && b1.first >= a2.first))) {
                if ((b1.second <= a1.second && a1.second <= b2.second) || (b1.second >= a1.second && a1.second >= b2.second)) {
                    // Intersection: a is horizontal, b is vertical

                    // Skip origin
                    if (a1.second == 0 && b1.first == 0) {
                        continue;
                    }

                    if (part2) {
                        minDist = std::min(minDist, steps1 + steps2 + abs(a1.second - b1.second) + abs(a1.first - b1.first));
                    } else {
                        minDist = std::min(minDist, abs(a1.second) + abs(b1.first));
                    }
                }
            } else if ((a1.second <= b1.second && b1.second <= a2.second) || (a1.second >= b1.second && b1.second >= a2.second)) {
                if ((b1.first <= a1.first && a1.first <= b2.first) || (b1.first >= a1.first && a1.first >= b2.first)) {
                    // Intersection: a is vertical, b is horizontal

                    // Skip origin
                    if (a1.first == 0 && b1.second == 0) {
                        continue;
                    }

                    if (part2) {
                        minDist = std::min(minDist, steps1 + steps2 + abs(a1.second - b1.second) + abs(a1.first - b1.first));
                    } else {
                        minDist = std::min(minDist, abs(a1.first) + abs(b1.second));
                    }
                }
            }

            // Only needed for part 2
            steps2 += abs(b1.first - b2.first) + abs(b1.second - b2.second);
        }

        // Only needed for part 2
        steps1 += abs(a1.first - a2.first) + abs(a1.second - a2.second);
        steps2 = 0;
    }

    return minDist;
}
