#include <iostream>
#include <cmath>
#include "solve.h"

#define PI 3.14159265358979323846

bool Day10::between(int a, int b, int c) {
    return (a <= b && b <= c) || (a >= b && b >= c);
}

int Day10::solve1(std::istream& input) {
    const auto asteroids = readAsteroids(input);

    int maxCount = 0;
    Asteroid maxStation(0, 0);

    for (auto station : asteroids) {
        auto events = getEvents(asteroids, station);
        int count = 0;
        double prevAngle = -1;

        while (!events.empty()) {
            auto event = events.top();
            events.pop();

            auto angle = std::get<0>(event);

            if (prevAngle == angle) {
                continue;
            }

            count++;
            prevAngle = angle;
        }

        if (count > maxCount) {
            maxCount = count;
            maxStation = station;
        }
    }

    std::cout << "Best location for new monitoring station: " << maxStation.x << ',' << maxStation.y << '\n';

    return maxCount;
}

int Day10::solve2(std::istream& input) {
    auto asteroids = readAsteroids(input);
    Asteroid station(13, 17);

    auto events = getEvents(asteroids, station);

    double prevAngle = -1;
    int index = 0;

    while (!events.empty()) {
        auto event = events.top();
        events.pop();

        auto angle = std::get<0>(event);
        auto dist = std::get<1>(event);
        auto asteroid = std::get<2>(event);

        if (prevAngle == angle) {
            events.emplace(angle + 360, dist, asteroid);
            continue;
        }

        index++;
        if (index == 200) {
            return asteroid.x * 100 + asteroid.y;
        }

        prevAngle = angle;
    }

    return -1;
}

namespace Day10 {

    std::vector<Asteroid> readAsteroids(std::istream& input) {
        std::vector<Asteroid> asteroids;

        int x = 0;
        int y = 0;
        while (!input.eof()) {
            switch (input.get()) {
                case '#':
                    asteroids.emplace_back(x, y);
                    // fallthrough
                case '.':
                    x++;
                    break;
                case '\n':
                    x = 0;
                    y++;
                    break;
            }
        }

        return asteroids;
    }

    std::priority_queue<Event, std::vector<Event>, std::greater<>>
    getEvents(const std::vector<Asteroid>& asteroids, Asteroid station) {
        std::priority_queue<Event, std::vector<Event>, std::greater<>> events;

        for (const auto& asteroid : asteroids) {
            if (asteroid == station) {
                continue;
            }

            double angle = (180 / PI) * atan2(asteroid.x - station.x, station.y - asteroid.y);
            if (angle < 0) {
                angle += 360;
            }

            int dist = std::abs(asteroid.x - station.x) + std::abs(asteroid.y - station.y);

            events.emplace(angle, dist, asteroid);
        }

        return events;
    }

}

