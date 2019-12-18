#include <set>
#include "solve.h"

namespace Day18 {

    int solve1(std::istream& input) {
        auto grid = parse_input(input);
        KeySet goal = find_keys(grid);
        return solve(grid, KeySet(), goal);
    }

    int solve2(std::istream& input) {
        auto grid = parse_input(input);
        KeySet goal = find_keys(grid);

        auto entrance = find_entrance(grid);
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                grid[{entrance.x + x, entrance.y + y}] = (x == 0 || y == 0) ? '#' : '@';
            }
        }

        int total_steps = 0;
        for (const auto& vault : split_grid(grid, entrance)) {
            KeySet start = find_keys(vault);
            start ^= goal;
            total_steps += solve(vault, start, goal);
        }
        return total_steps;
    }

    void insert_key(KeySet& key_set, Key key) {
        key_set[key - 'a'] = true;
    }

    bool get_key(const KeySet& key_set, Key key) {
        return key_set[key - 'a'];
    }

    Grid<char> parse_input(std::istream& input) {
        Grid<char> grid;

        int x = 0;
        int y = 0;
        while (input.peek() != EOF) {
            char c = input.get();

            if (c == '\n') {
                y++;
                x = 0;
                continue;
            }

            grid[{x, y}] = c;
            x++;
        }

        return grid;
    }

    Coord find_entrance(const Grid<char>& grid) {
        for (auto kv : grid) {
            if (kv.second == '@') {
                return kv.first;
            }
        }
        throw std::logic_error("entrance not found");
    }

    KeySet find_keys(const Grid<char>& grid) {
        KeySet keys;
        for (auto kv : grid) {
            if ('a' <= kv.second && kv.second <= 'z') {
                insert_key(keys, kv.second);
            }
        }
        return keys;
    }

    int solve(const Grid<char>& grid, const KeySet& start, const KeySet& goal) {
        auto entrance = find_entrance(grid);

        using Node = std::pair<Coord, KeySet>;
        std::queue<Node> queue;
        std::unordered_map<Node, int> cost;

        queue.emplace(entrance, start);
        cost.emplace(queue.front(), 0);

        while (!queue.empty()) {
            const auto node = queue.front();
            queue.pop();

            // Found all keys
            if (node.second == goal) {
                return cost[node];
            }

            for (int i = 1; i <= 4; ++i) {
                auto dir = static_cast<Direction>(i);

                auto neighbor = node;
                neighbor.first = node.first.step(dir);

                auto c = grid.at(neighbor.first);

                // Skip walls
                if (c == '#') {
                    continue;
                }

                // Skip closed doors
                if ('A' <= c && c <= 'Z') {
                    if (!get_key(node.second, tolower(c))) {
                        continue;
                    }
                }

                // Collect keys
                if ('a' <= c && c <= 'z') {
                    if (!get_key(node.second, c)) {
                        insert_key(neighbor.second, c);
                    }
                }

                // Already discovered this node
                if (cost.count(neighbor) != 0) {
                    continue;
                }
                cost.emplace(neighbor, cost[node] + 1);
                queue.push(neighbor);
            }
        }
    }

    std::vector<Grid<char>> split_grid(const Grid<char>& grid, const Coord& split_point) {
        std::vector<Grid<char>> grids(4);

        for (auto kv : grid) {
            if (kv.first.x >= split_point.x && kv.first.y >= split_point.y) {
                grids[0].insert(kv);
            }
            if (kv.first.x >= split_point.x && kv.first.y <= split_point.y) {
                grids[1].insert(kv);
            }
            if (kv.first.x <= split_point.x && kv.first.y >= split_point.y) {
                grids[2].insert(kv);
            }
            if (kv.first.x <= split_point.x && kv.first.y <= split_point.y) {
                grids[3].insert(kv);
            }
        }

        return grids;
    }
}
