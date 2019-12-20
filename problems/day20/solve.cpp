#include <set>
#include <iostream>
#include "solve.h"

namespace Day20 {

    inline bool is_portal(char c) {
        return 'A' <= c && c <= 'Z';
    }

    Direction reverse(Direction direction) {
        switch (direction) {
            case North:
                return South;
            case South:
                return North;
            case West:
                return East;
            case East:
                return West;
        }
    }

    int solve1(std::istream& input) {
        return solve(input, false);
    }

    int solve2(std::istream& input) {
        return solve(input, true);
    }

    int solve(std::istream& input, bool part2) {
        auto grid = parse_input(input);

        Coord start_coord;
        Coord goal_coord;

        // Maps coordinates in front of portals with the same label
        std::unordered_map<Coord, Coord> portals;
        // Whether the coordinates are in front of a portal on the outer edge
        std::unordered_map<Coord, bool> is_outer_edge;

        {
            // Temporary map for storing portals for which we haven't found the second one yet
            std::unordered_map<std::string, Coord> portals_tmp;
            for (auto kv : grid) {
                if (!is_portal(kv.second)) {
                    continue;
                }

                auto n1 = kv.first;

                for (int i = 1; i <= 4; ++i) {
                    auto dir = static_cast<Direction>(i);
                    auto n2 = n1.step(dir);

                    if (!grid.count(n2) || !is_portal(grid[n2])) {
                        continue;
                    }

                    auto n3 = n2.step(dir);
                    if (!grid.count(n3) || grid[n3] != '.') {
                        continue;
                    }

                    auto c1 = kv.second;
                    auto c2 = grid[n2];

                    std::string label;
                    label += (dir == South || dir == East) ? c1 : c2;
                    label += (dir == South || dir == East) ? c2 : c1;

                    if (label == "AA") {
                        start_coord = n3;
                        continue;
                    }

                    if (label == "ZZ") {
                        goal_coord = n3;
                        continue;
                    }

                    if (portals_tmp.count(label)) {
                        // This is the second portal with this label, resolve them
                        portals[n3] = portals_tmp[label];
                        portals[portals_tmp[label]] = n3;
                    } else {
                        // First portal with this label
                        portals_tmp[label] = n3;
                    }

                    // Abuse the fact that labels of portals on the outer edge also lie on the edge of the grid
                    is_outer_edge[n3] = !grid.count(n1.step(reverse(dir)));
                }
            }
        }

        // BFS
        using Node = std::pair<Coord, int>;

        std::queue<Node> queue;
        std::unordered_map<Node, int> cost;

        const Node start_node = {start_coord, 0};
        const Node goal_node = {goal_coord, 0};
        queue.push(start_node);
        cost[start_node] = 0;

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            if (current == goal_node) {
                return cost[current];
            }

            std::vector<Node> neighbors;

            // Neighbors from just stepping into any of the four directions
            for (auto n : current.first.get_neighbors()) {
                neighbors.emplace_back(n, current.second);
            }

            // Neighbor from stepping through a portal
            if (portals.count(current.first)) {
                int level = current.second;
                if (part2) { // Only change levels in Part 2
                    if (is_outer_edge[current.first]) {
                        --level;
                    } else {
                        ++level;
                    }
                }
                neighbors.emplace_back(portals[current.first], level);
            }

            for (auto neighbor : neighbors) {
                // Skip walls and labels
                if (grid[neighbor.first] == '#' || is_portal(grid[neighbor.first])) {
                    continue;
                }

                if (neighbor.second < 0) {
                    continue;
                }

                if (cost.count(neighbor)) {
                    continue;
                }

                cost[neighbor] = cost[current] + 1;
                queue.push(neighbor);
            }
        }
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
}
