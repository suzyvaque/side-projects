cmake_minimum_required(VERSION 3.26)
project(forsnusolo_aus_new)

set(CMAKE_CXX_STANDARD 14)

add_executable(forsnusolo_aus_new
        main.cpp
        StaticData.h
        StaticData.cpp
        Status.h
        Status.cpp
        ByReg.h
        ByReg.cpp
        Car.h
        Car.cpp
)
