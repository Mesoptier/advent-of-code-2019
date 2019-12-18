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

        // Replace squares around the original entrance
        auto entrance = find_entrance(grid);
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                grid[{entrance.x + x, entrance.y + y}] = (x == 0 || y == 0) ? '#' : '@';
            }
        }

        // Solve each vault separately, assuming all doors without the key in the same vault are always opened.
        // The final answer is the sum of the answer of each vault.
        int total_steps = 0;
        for (const auto& vault : split_grid(grid, entrance)) {
            KeySet start = find_keys(vault);
            start ^= goal;
            total_steps += solve(vault, start, goal);
        }
        return total_steps;
    }

    Grid<char> parse_input(std::istream& input) {
        Grid<char> grid;

        int x = 0;
        int y = 0;
        while (input.peek() != EOF) {
            char c = input.get();

            if (c == '\n') {
                ++y;
                x = 0;
                continue;
            }

            grid[{x, y}] = c;
            ++x;
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
        // Compute adjacency list
        std::unordered_map<char, std::vector<std::pair<char, int>>> adjacency;
        for (auto kv : grid) {
            if (kv.second == '@' || is_key(kv.second) || is_door(kv.second)) {
                adjacency[kv.second] = reachable_nodes(grid, kv.first);
            }
        }

        using Node = std::pair<char, KeySet>;
        std::priority_queue<PriorityNode<int, Node>, std::vector<PriorityNode<int, Node>>, std::greater<>> queue;
        std::unordered_map<Node, int> cost;

        Node start_node = {'@', start};
        queue.emplace(0, start_node);
        cost.emplace(start_node, 0);

        while (!queue.empty()) {
            auto pq_node = queue.top();
            queue.pop();

            auto current = pq_node.node;
            auto current_cost = pq_node.cost;

            if (cost[current] < current_cost) {
                continue;
            }

            // Found all keys
            if (current.second == goal) {
                return cost[current];
            }

            for (const auto& adj_node : adjacency[current.first]) {
                auto neighbor = current;
                neighbor.first = adj_node.first;

                if (is_door(neighbor.first)) {
                    // Skip closed doors
                    if (!get_key(current.second, tolower(neighbor.first))) {
                        continue;
                    }
                } else if (is_key(neighbor.first)) {
                    // Collect keys
                    if (!get_key(current.second, neighbor.first)) {
                        insert_key(neighbor.second, neighbor.first);
                    }
                }

                int neighbor_cost = current_cost + adj_node.second;

                auto it = cost.find(neighbor);
                if (it != cost.end() && it->second < neighbor_cost) {
                    continue;
                }

                cost.emplace(neighbor, neighbor_cost);
                queue.emplace(neighbor_cost, neighbor);
            }
        }

        return -1;
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

    std::vector<std::pair<char, int>> reachable_nodes(const Grid<char>& grid, const Coord& from) {
        std::vector<std::pair<char, int>> nodes;

        std::queue<Coord> queue;
        std::unordered_map<Coord, int> cost;

        queue.push(from);
        cost[from] = 0;

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            for (int i = 1; i <= 4; ++i) {
                auto dir = static_cast<Direction>(i);
                auto neighbor = current.step(dir);

                auto c = grid.at(neighbor);

                // Skip walls
                if (c == '#') continue;

                // Skip discovered nodes
                if (cost.count(neighbor) != 0) continue;

                cost[neighbor] = cost[current] + 1;

                if (is_key(c) || is_door(c)) {
                    nodes.emplace_back(c, cost[neighbor]);
                    continue;
                }

                queue.push(neighbor);
            }
        }

        return nodes;
    }

    bool is_key(char c) {
        return 'a' <= c && c <= 'z';
    }

    bool is_door(char c) {
        return 'A' <= c && c <= 'Z';
    }
}
