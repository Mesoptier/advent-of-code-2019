#pragma once

#include <istream>
#include <utility>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>

namespace Day19 {

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
    };

    template<class V>
    using Grid = std::map<Coord, V>;

    void print_grid(const Grid<char>& grid);

    struct Program {
        using Int = long long int;

        std::map<Int, Int> memory;
        Int ip = 0; // Instruction pointer
        Int rb = 0; // Relative base
        std::queue<Int> output;
        std::queue<Int> input;
        bool halted = false;

        explicit Program(std::istream& input);

        Int& at(Int index);

        bool run();
    };

    int solve1(std::istream& input);
    int solve2(std::istream& input);

    bool get_state(const Program& program, Coord pos);
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
    struct hash<Day19::Coord> {
        size_t operator()(const Day19::Coord& coord) const {
            size_t s = 0;
            hash_combine(s, coord.x);
            hash_combine(s, coord.y);
            return s;
        }
    };
}