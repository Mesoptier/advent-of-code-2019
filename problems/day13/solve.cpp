#include <iostream>
#include <wtypes.h>
#include <wincon.h>
#include <chrono>
#include <thread>
#include "solve.h"

#define WIDTH 40
#define HEIGHT 24

namespace Day13 {

    int solve1(std::istream& input) {
        Program program(input);
        std::map<Coord, int> grid;

        while (!program.halted) {
            program.run();
        }

        while (!program.output.empty()) {
            int x = program.output.front();
            program.output.pop();
            int y = program.output.front();
            program.output.pop();
            int tileId = program.output.front();
            program.output.pop();

            grid[{x, y}] = tileId;
        }

        int count = 0;

        for (const auto& kv : grid) {
            if (kv.second == 2) {
                count++;
            }
        }

        return count;
    }

    int solve2(std::istream& input) {
        Program program(input);
        std::map<Coord, int> grid;
        int score = 0;

        Coord paddlePos = {0, 0};
        Coord ballPos = {0, 0};

        program.at(0) = 2;

        while (!program.halted) {
            program.run();

            while (!program.output.empty()) {
                int x = program.output.front();
                program.output.pop();
                int y = program.output.front();
                program.output.pop();
                int tileId = program.output.front();
                program.output.pop();

                if (x == -1 && y == 0) {
                    score = tileId;
                } else {
                    grid[{x, y}] = tileId;

                    if (tileId == 4) {
                        ballPos = {x, y};
                    }
                    if (tileId == 3) {
                        paddlePos = {x, y};
                    }
                }
            }

            // Draw grid
            renderGrid(grid, score);
            std::this_thread::sleep_for(std::chrono::milliseconds(16));

            // Move paddle to be below ball
            if (ballPos.x > paddlePos.x) {
                program.input.push(1);
            } else if (ballPos.x < paddlePos.x) {
                program.input.push(-1);
            } else {
                program.input.push(0);
            }
        }

        return score;
    }

    void renderGrid(const Grid& grid, int score) {
        SMALL_RECT windowSize = { 0, 0, WIDTH - 1, HEIGHT - 1 };
        COORD bufferSize = { WIDTH, HEIGHT };

        COORD characterBufferSize = { WIDTH, HEIGHT };
        COORD characterPosition = { 0, 0 };
        SMALL_RECT consoleWriteArea = { 0, 0, WIDTH - 1, HEIGHT - 1 };

        CHAR_INFO consoleBuffer[WIDTH * HEIGHT];

        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

        char title[50];
        sprintf(title, "AoC - Score: %d", score);
        SetConsoleTitle(title);
        SetConsoleWindowInfo(output, TRUE, &windowSize);
        SetConsoleScreenBufferSize(output, bufferSize);

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                unsigned char tile;
                WORD attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

                switch (grid.at({x, y})) {
                    case 1: // wall
                        if (y == 0) {
                            if (x == 0) {
                                tile = 201;
                            } else if (x == WIDTH - 1) {
                                tile = 187;
                            } else {
                                tile = 205;
                            }
                        } else {
                            tile = 186;
                        }
                        break;
                    case 2: // block
                        tile = 219;
                        switch (y % 3) {
                            case 0:
                                attr ^= FOREGROUND_BLUE;
                                break;
                            case 1:
                                attr ^= FOREGROUND_GREEN;
                                break;
                            case 2:
                                attr ^= FOREGROUND_RED;
                                break;
                        }
                        break;
                    case 3: // horizontal paddle
                        tile = 220;
                        break;
                    case 4: // ball
                        tile = 254;
                        break;
                    case 0: // empty
                    default:
                        tile = 0;
                        break;
                }

                consoleBuffer[x + WIDTH * y].Char.UnicodeChar = tile;
                consoleBuffer[x + WIDTH * y].Attributes = attr;
            }
        }

        WriteConsoleOutputA(output, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
    }

    Program::Program(std::istream& input) {
        Int index = 0;
        while (input) {
            std::string s;
            if (!std::getline(input, s, ',')) {
                break;
            }
            memory.insert({index++, std::stoll(s)});
        }
    }

    Program::Int& Program::at(Int index) {
        auto it = memory.find(index);
        if (it == memory.end()) {
            return memory.insert({index, 0}).first->second;
        }
        return it->second;
    }

    bool Program::run() {
        // Number of parameters for each instruction
        int nParams[] = {0, 3, 3, 1, 1, 2, 2, 3, 3, 1};

        while (at(ip) != 99) {
            int instruction = at(ip);

            // Get two-digit opcode
            int opcode = instruction % 100;
            instruction /= 100;

            // Get parameters
            std::vector<Int> params; // Raw parameters
            std::vector<Int> values; // Handled based parameter mode
            for (int i = 0; i < nParams[opcode]; ++i) {
                int mode = instruction % 10;
                instruction /= 10;

                Int param = at(ip + 1 + i);

                switch (mode) {
                    case 0: // Position mode
                        params.push_back(param);
                        values.push_back(at(param));
                        break;
                    case 1: // Immediate mode
                        params.push_back(param);
                        values.push_back(param);
                        break;
                    case 2: // Relative mode
                        params.push_back(rb + param);
                        values.push_back(at(rb + param));
                        break;
                }
            }

            bool ipModified = false;

            switch (opcode) {
                case 1: // add
                    at(params[2]) = values[0] + values[1];
                    break;
                case 2: // multiply
                    at(params[2]) = values[0] * values[1];
                    break;
                case 3: // input
                    if (input.empty()) {
                        return true;
                    }
                    at(params[0]) = input.front();
                    input.pop();
                    break;
                case 4: // output
                    output.push(values[0]);
                    break;
                case 5: // jump-if-true
                    if (values[0] != 0) {
                        ip = values[1];
                        ipModified = true;
                    }
                    break;
                case 6: // jump-if-false
                    if (values[0] == 0) {
                        ip = values[1];
                        ipModified = true;
                    }
                    break;
                case 7: // less than
                    at(params[2]) = values[0] < values[1];
                    break;
                case 8: // equals
                    at(params[2]) = values[0] == values[1];
                    break;
                case 9: // adjust relative base
                    rb += values[0];
                    break;
                default:
                    throw std::logic_error("Unknown opcode");
            }

            if (!ipModified) {
                ip += nParams[opcode] + 1;
            }
        }

        halted = true;
        return false;
    }

}
