cmake_minimum_required(VERSION 3.13.0)
project(AdventOfCode2019)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

add_executable(App
        main.cpp
        problems/day24/solve.cpp
        )

target_include_directories(App PRIVATE util)