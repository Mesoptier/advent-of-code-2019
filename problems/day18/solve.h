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

    bool is_key(char c);
    bool is_door(char c);

    using Node = uint32_t;
    using Keyset = uint32_t;

    using NodeKeyset = uint64_t;
    constexpr uint64_t NODE_MASK = (~0ULL << 32u);
    constexpr uint64_t KEYSET_MASK = ~NODE_MASK;

    inline void set_node(NodeKeyset& nks, Node node) {
        nks = (nks & KEYSET_MASK) | ((NodeKeyset) node << 32u);
    }
    inline Node get_node(NodeKeyset nks) {
        return nks >> 32u;
    }
    inline void set_keyset(NodeKeyset& nks, Keyset keyset) {
        nks = (nks & NODE_MASK) | keyset;
    }
    inline Keyset get_keyset(NodeKeyset nks) {
        return nks;
    }

    // Keys:
    // a = 0b0010...01
    // b = 0b0010...10
    // ...
    // z = 0b0011...00
    //
    // Doors:
    // A = 0b0100...01
    // B = 0b0100...10
    // ...
    // Z = 0b0101...00
    //
    // Entrance:
    // @ = 0b1000...00

    constexpr uint32_t KEY_BIT = 1u << 26u;
    constexpr uint32_t DOOR_BIT = 1u << 27u;
    constexpr uint32_t ENTRANCE_BIT = 1u << 28u;

    inline Node node_key(char c) {
        return KEY_BIT + (1u << (uint32_t) (c - 'a'));
    }
    inline Node node_door(char c) {
        return DOOR_BIT + (1u << (uint32_t) (c - 'A'));
    }
    inline Node node_entrance() {
        return ENTRANCE_BIT;
    }
    inline Node node(char c) {
        if (c == '@') return node_entrance();
        if (is_key(c)) return node_key(c);
        if (is_door(c)) return node_door(c);
        throw std::invalid_argument("invalid node char");
    }

    inline bool is_key(Node node) {
        return node & KEY_BIT;
    }
    inline bool is_door(Node node) {
        return node & DOOR_BIT;
    }
    inline bool is_entrance(Node node) {
        return node & ENTRANCE_BIT;
    }

    inline Keyset insert_key(Keyset keys, Node key) {
        // assert(is_key(key));
        return keys | (key & ~KEY_BIT);
    }
    inline bool has_key(Keyset keys, Node key) {
        // assert(is_key(key) || is_door(key));
        return keys & key;
    }

    Grid<char> parse_input(std::istream& input);
    Coord find_entrance(const Grid<char>& grid);
    Keyset find_keys(const Grid<char>& grid);
    int solve(const Grid<char>& grid, const Keyset& start, const Keyset& goal);
    std::vector<Grid<char>> split_grid(const Grid<char>& grid, const Coord& split_point);
    std::vector<std::pair<Node, int>> reachable_nodes(const Grid<char>& grid, const Coord& from);

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