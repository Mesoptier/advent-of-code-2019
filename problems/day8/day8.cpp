#include <iostream>
#include "day8.h"

int Day8::solve1(std::istream& input) {
    int min = INT_MAX;
    int result = 0;

    while (input.peek() != EOF) {
        int count[3]{};

        for (int y = 0; y < DAY8_HEIGHT; ++y) {
            for (int x = 0; x < DAY8_WIDTH; ++x) {
                int pixel = input.get() - 48;
                count[pixel]++;
            }
        }

        if (count[0] < min) {
            min = count[0];
            result = count[1] * count[2];
        }
    }

    return result;
}

int Day8::solve2(std::istream& input) {
    // Slightly modified pixel values, so I don't have to manually fill the
    // image array with 2's:
    // 0 - transparent
    // 1 - black
    // 2 - white

    int image[DAY8_HEIGHT][DAY8_WIDTH]{};

    while (input.peek() != EOF) {
        for (auto& row : image) {
            for (int& pixel : row) {
                int inputPixel = input.get() - 48;
                if (pixel == 0) {
                    pixel = (inputPixel + 1) % 3;
                }
            }
        }
    }

    for (auto& row : image) {
        for (int pixel : row) {
            std::cout << (pixel == 1 ? ' ' : '#');
        }
        std::cout << std::endl;
    }

    return 0;
}

