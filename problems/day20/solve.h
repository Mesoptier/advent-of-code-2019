#pragma once

#include <istream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <bitset>
#include <ostream>

namespace Day20 {

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

        [[nodiscard]] Coord step(Direction direction) const {
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

        [[nodiscard]] std::vector<Coord> get_neighbors() const {
            return {{
                {x + 1, y},
                {x - 1, y},
                {x, y + 1},
                {x, y - 1},
            }};
        }
    };

    template<class V>
    using Grid = std::unordered_map<Coord, V>;

    Grid<char> parse_input(std::istream& input);

    int solve1(std::istream& input);
    int solve2(std::istream& input);

    int solve(std::istream& input, bool part2);
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
    struct hash<Day20::Coord> {
        size_t operator()(const Day20::Coord& coord) const {
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