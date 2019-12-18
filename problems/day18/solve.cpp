#include <set>
#include "solve.h"

namespace Day18 {

    int solve1(std::istream& input) {
        auto grid = parse_input(input);
        Keyset goal = find_keys(grid);
        return solve(grid, 0, goal);
    }

    int solve2(std::istream& input) {
        auto grid = parse_input(input);
        Keyset goal = find_keys(grid);

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
            Keyset start = find_keys(vault);
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

    Keyset find_keys(const Grid<char>& grid) {
        Keyset keys = 0;
        for (auto kv : grid) {
            if (is_key(kv.second)) {
                keys = insert_key(keys, node_key(kv.second));
            }
        }
        return keys;
    }

    int solve(const Grid<char>& grid, const Keyset& start, const Keyset& goal) {
        // Compute adjacency list
        std::unordered_map<Node, std::vector<std::pair<Node, int>>> adjacency;
        for (auto kv : grid) {
            if (kv.second == '@' || is_key(kv.second) || is_door(kv.second)) {
                adjacency[node(kv.second)] = reachable_nodes(grid, kv.first);
            }
        }

        std::priority_queue<PriorityNode<int, NodeKeyset>, std::vector<PriorityNode<int, NodeKeyset>>, std::greater<>> queue;
        std::unordered_map<NodeKeyset, int> cost;

        NodeKeyset start_node = 0;
        set_node(start_node, node_entrance());
        set_keyset(start_node, start);

        queue.emplace(0, start_node);
        cost.emplace(start_node, 0);

        while (!queue.empty()) {
            const auto current = queue.top().node;
            const auto current_cost = queue.top().cost;
            queue.pop();

            const auto current_keyset = get_keyset(current);

            if (cost[current] < current_cost) {
                continue;
            }

            // Found all keys
            if (current_keyset == goal) {
                return cost[current];
            }

            for (const auto& adj_node : adjacency[get_node(current)]) {
                auto neighbor = current;
                Node node = adj_node.first;
                set_node(neighbor, node);

                if (!has_key(current_keyset, node)) {
                    if (is_door(node)) {
                        // Skip closed doors
                        continue;
                    } else /*if (is_key(node))*/ {
                        // Collect keys
                        set_keyset(neighbor, insert_key(current_keyset, node));
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

    std::vector<std::pair<Node, int>> reachable_nodes(const Grid<char>& grid, const Coord& from) {
        std::vector<std::pair<Node, int>> nodes;

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
                    nodes.emplace_back(node(c), cost[neighbor]);
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
