#pragma once

#include <istream>
#include <vector>
#include <map>
#include <queue>

namespace Day10 {

    struct Asteroid
    {
        int x;
        int y;

        Asteroid(int x, int y) : x(x), y(y) {};

        bool operator==(const Asteroid& rhs) const {
            return x == rhs.x &&
                y == rhs.y;
        }
        bool operator!=(const Asteroid& rhs) const {
            return !(rhs == *this);
        }
        bool operator<(const Asteroid& rhs) const {
            if (x < rhs.x)
                return true;
            if (rhs.x < x)
                return false;
            return y < rhs.y;
        }
        bool operator>(const Asteroid& rhs) const {
            return rhs < *this;
        }
        bool operator<=(const Asteroid& rhs) const {
            return !(rhs < *this);
        }
        bool operator>=(const Asteroid& rhs) const {
            return !(*this < rhs);
        }
    };

    using Event = std::tuple<double, int, Asteroid>;

    std::vector<Asteroid> readAsteroids(std::istream& input);
    bool between(int a, int b, int c);

    std::priority_queue<Event, std::vector<Event>, std::greater<>>
    getEvents(const std::vector<Asteroid>& asteroids, Asteroid station);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
