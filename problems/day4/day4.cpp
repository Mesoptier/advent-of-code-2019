#include <iostream>
#include "day4.h"

int Day4::solve1(std::istream& input) {
    return solveBoth(input, false);
}

int Day4::solve2(std::istream& input) {
    return solveBoth(input, false);
}

int Day4::solveBoth(std::istream& input, bool part2) {
    // Read input
    int min, max;
    input >> min;
    input.ignore(); // Ignore - between numbers
    input >> max;

    // Compute number of valid passwords
    int count = 0;
    for (int password = min; password <= max; ++password) {
        if (checkPassword(password, part2)) {
            count++;
        }
    }
    return count;
}

bool Day4::checkPassword(int password, bool part2) {
    int prevDigit = 10;
    int repeat = 1;
    bool hasRepeat = false;

    do {
        // Get digits from password, starting at 1's -> 10's -> 100's etc.
        int digit = password % 10;

        if (prevDigit < digit) {
            // Digit to the right is smaller than current digit -> Invalid password
            return false;
        } else if (digit == prevDigit) {
            // Repeated digit
            repeat++;
        } else {
            // In part 1 we need a repeated group of at least length 2
            // In part 2 we need a repeated group of exactly length 2
            hasRepeat = hasRepeat || (!part2 && repeat >= 2) || (part2 && repeat == 2);

            // Starting a new repeated group with the current character
            repeat = 1;
        }

        prevDigit = digit;
        password /= 10;
    } while (password > 0);

    // Deal with final repeated group
    hasRepeat = hasRepeat || (!part2 && repeat >= 2) || (part2 && repeat == 2);

    return hasRepeat;
}