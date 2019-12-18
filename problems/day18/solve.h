#pragma once

#include <istream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <bitset>
#include <ostream>

namespace Day18 {
    enum Direction {
        North = 1,
        South = 2,
        West = 3,
        East = 4,
    };

    struct Coord {
        int x;
        int y;

        bool operator==(const Coord& rhs) const {
            return x == rhs.x &&
                y == rhs.y;
        }
        bool operator!=(const Coord& rhs) const {
            return !(rhs == *this);
        }
        bool operator<(const Coord& rhs) const {
            if (x < rhs.x)
                return true;
            if (rhs.x < x)
                return false;
            return y < rhs.y;
        }
        bool operator>(const Coord& rhs) const {
            return rhs < *this;
        }
        bool operator<=(const Coord& rhs) const {
            return !(rhs < *this);
        }
        bool operator>=(const Coord& rhs) const {
            return !(*this < rhs);
        }

        Coord step(Direction direction) const {
            switch (direction) {
                case North:
                    return { x, y - 1 };
                case South:
                    return { x, y + 1 };
                case West:
                    return { x - 1, y };
                case East:
                    return { x + 1, y };
                default:
                    throw std::invalid_argument("invalid direction");
            }
        }
    };

    template<class V>
    using Grid = std::unordered_map<Coord, V>;

    template<class Cost, class Node>
    struct PriorityNode {
        Cost cost;
        Node node;

        PriorityNode(Cost cost, Node node) : cost(cost), node(node) {}

        bool operator<(const PriorityNode& rhs) const {
            return cost < rhs.cost;
        }
        bool operator>(const PriorityNode& rhs) const {
            return rhs < *this;
        }
        bool operator<=(const PriorityNode& rhs) const {
            return !(rhs < *this);
        }
        bool operator>=(const PriorityNode& rhs) const {
            return !(*this < rhs);
        }
    };

//    template<class Cost, class Node>
//    bool operator>(const PriorityNode<Cost, Node>& a, const PriorityNode<Cost, Node>& b);

    using Key = char;
    using KeySet = std::bitset<26>;

    inline void insert_key(KeySet& key_set, Key key) {
        key_set[key - 'a'] = true;
    }

    inline bool get_key(const KeySet& key_set, Key key) {
        return key_set[key - 'a'];
    }

    Grid<char> parse_input(std::istream& input);
    Coord find_entrance(const Grid<char>& grid);
    KeySet find_keys(const Grid<char>& grid);
    int solve(const Grid<char>& grid, const KeySet& start, const KeySet& goal);
    std::vector<Grid<char>> split_grid(const Grid<char>& grid, const Coord& split_point);
    std::vector<std::pair<char, int>> reachable_nodes(const Grid<char>& grid, const Coord& from);

    bool is_key(char c);
    bool is_door(char c);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}

namespace {
    template <class T>
    inline void hash_combine(std::size_t& s, const T & v) {
        std::hash<T> h;
        s^= h(v) + 0x9e3779b9 + (s<< 6) + (s>> 2);
    }
}

namespace std {
    template<>
    struct hash<Day18::Coord> {
        size_t operator()(const Day18::Coord& coord) const {
            size_t s = 0;
            hash_combine(s, coord.x);
            hash_combine(s, coord.y);
            return s;
        }
    };

    template<class T1, class T2>
    struct hash<pair<T1, T2>> {
        size_t operator()(const pair<T1, T2> p) const {
            size_t s = 0;
            hash_combine(s, p.first);
            hash_combine(s, p.second);
            return s;
        }
    };
}