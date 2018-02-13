cmake_minimum_required(VERSION 3.0)
project(BigIntegerTest)

if (CMAKE_COMPILER_IS_GNUCXX OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang"))
    set(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(BigIntegerTest
    main.cpp
    BigInteger.hpp
    BigInteger.cpp
)
