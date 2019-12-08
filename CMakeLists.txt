cmake_minimum_required(VERSION 3.13.0)
project(AdventOfCode2019)

set(CMAKE_CXX_STANDARD 17)

add_executable(App
        main.cpp
        problems/day1/day1.cpp
        problems/day2/day2.cpp
        problems/day3/day3.cpp
        problems/day4/day4.cpp
        problems/day5/day5.cpp
        problems/day6/day6.cpp
        problems/day7/day7.cpp
        problems/day8/day8.cpp
        )