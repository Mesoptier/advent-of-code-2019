#pragma once

#include <istream>
#include <vector>
#include <map>
#include <queue>
#include <ostream>

namespace Day12 {

    struct Vec {
        int x;
        int y;
        int z;

        Vec(int x, int y, int z) : x(x), y(y), z(z) {};

        int& operator[](size_t n) {
            if (n == 0) return x;
            if (n == 1) return y;
            if (n == 2) return z;
            throw std::invalid_argument("n should be 0, 1, or 2");
        }

        Vec& operator+=(const Vec& other) {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vec& vec) {
            os << "x=" << vec.x << " y=" << vec.y << " z=" << vec.z;
            return os;
        }

        int energy() const {
            return std::abs(x) + std::abs(y) + std::abs(z);
        }
    };

    struct Moon {
        Vec ipos;
        Vec pos;
        Vec vel;

        Moon(int ix, int iy, int iz): ipos(ix, iy, iz), pos(ipos), vel(0, 0, 0) {};

        int pot() const {
            return pos.energy();
        }
        int kin() const {
            return vel.energy();
        }
    };

    using State = std::vector<Moon>;
    State readInput(std::istream& input);
    void simulate(State& state);

    int solve1(std::istream& input);
    long long int solve2(std::istream& input);
}
