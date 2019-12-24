#include <set>
#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_map>
#include <bitset>
#include <map>
#include "solve.h"

namespace Day24 {

    int solve1(std::istream& input) {
        constexpr int w = 5;
        constexpr int h = 5;
        using grid_t = std::bitset<w * h>;

        grid_t grid(w * h);
        std::set<unsigned int> ratings;

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                grid[y * w + x] = (input.get() == '#');
            }
            input.get();
        }

        while (true) {
            auto next_grid = grid;

            for (int x = 0; x < w; ++x) {
                for (int y = 0; y < h; ++y) {
                    bool c = grid[y * w + x];
                    int adj_bugs = 0;

                    for (int dx = -1; dx <= 1; ++dx) {
                        for (int dy = -1; dy <= 1; ++dy) {
                            if ((dx == 0) == (dy == 0)) {
                                continue;
                            }
                            if (y+dy < 0 || y+dy >= h || x+dx < 0 || x+dx >= w) {
                                continue;
                            }
                            if (grid[(y + dy) * w + (x + dx)]) {
                                ++adj_bugs;
                            }
                        }
                    }

                    if (c && adj_bugs != 1) {
                        next_grid[y * w + x] = false;
                    } else if (!c && 1 <= adj_bugs && adj_bugs <= 2) {
                        next_grid[y * w + x] = true;
                    }
                }
            }

            uint32_t rating = grid.to_ulong();
            if (ratings.count(rating)) {
                return rating;
            }

            ratings.insert(rating);
            swap(grid, next_grid);
        }
    }

    int solve2(std::istream& input) {
        constexpr int w = 5;
        constexpr int h = 5;
        using layer_t = std::bitset<w * h>;

        std::map<int, layer_t> layers;
        int z_min = 0;
        int z_max = 0;

        // Read input
        layers.emplace();
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                layers.at(0)[y * w + x] = (input.get() == '#');
            }
            input.get();
        }

        auto adj_bugs = [&](int x, int y, int z) {
            auto layer = layers.at(z);
            auto layer_in = layers.count(z - 1) ? layers.at(z - 1) : layer_t();
            auto layer_out = layers.count(z + 1) ? layers.at(z + 1) : layer_t();

            int count = 0;

            // Count north
            if (y == 0) {
                count += layer_out[1 * w + 2];
            } else if (y == 3 && x == 2) {
                for (int ix = 0; ix < 5; ++ix) {
                    count += layer_in[(4 * w) + ix];
                }
            } else {
                count += layer[(y - 1) * w + x];
            }

            // Count south
            if (y == 4) {
                count += layer_out[3 * w + 2];
            } else if (y == 1 && x == 2) {
                for (int ix = 0; ix < 5; ++ix) {
                    count += layer_in[(0 * w) + ix];
                }
            } else {
                count += layer[(y + 1) * w + x];
            }

            // Count west
            if (x == 0) {
                count += layer_out[2 * w + 1];
            } else if (y == 2 && x == 3) {
                for (int iy = 0; iy < 5; ++iy) {
                    count += layer_in[(iy * w) + 4];
                }
            } else {
                count += layer[y * w + (x - 1)];
            }

            // Count east
            if (x == 4) {
                count += layer_out[2 * w + 3];
            } else if (y == 2 && x == 1) {
                for (int iy = 0; iy < 5; ++iy) {
                    count += layer_in[(iy * w) + 0];
                }
            } else {
                count += layer[y * w + (x + 1)];
            }

            return count;
        };

        for (int minute = 0; minute < 200; ++minute) {
            auto next_layers = layers;

            // Add new inner/outer layers if the current inner/outer layers
            // contain any bugs, since they could have an effect on new layers.
            if (next_layers.at(z_min).any()) {
                next_layers.emplace(z_min - 1, layer_t());
                --z_min;
            }
            if (next_layers.at(z_max).any()) {
                next_layers.emplace(z_max + 1, layer_t());
                ++z_max;
            }

            for (int z = z_min; z <= z_max; ++z) {
                for (int y = 0; y < h; ++y) {
                    for (int x = 0; x < w; ++x) {
                        // Skip center
                        if (y == 2 && x == 2) {
                            continue;
                        }

                        bool c = layers[z][y * w + x];
                        int count = adj_bugs(x, y, z);

                        if (c && count != 1) {
                            next_layers[z][y * w + x] = false;
                        } else if (!c && 1 <= count && count <= 2) {
                            next_layers[z][y * w + x] = true;
                        }
                    }
                }
            }

            swap(layers, next_layers);
        }

        int count = 0;
        for (auto kv : layers) {
            count += kv.second.count();
        }
        return count;
    }

}
