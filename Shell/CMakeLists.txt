cmake_minimum_required(VERSION 3.0)
project(Shell)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -g ${CMAKE_CXX_FLAGS}")
endif()

add_executable(Shell
    main.cpp
    Shell.hpp
    Shell.cpp
)
