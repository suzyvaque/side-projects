cmake_minimum_required(VERSION 3.26)
project(always_unit_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(always_unit_test
        StaticData.cpp
        StaticData.h
        main.cpp)
