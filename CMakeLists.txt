cmake_minimum_required(VERSION 3.10)

project(HFTBacktest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

add_executable(main src/csv.cpp src/book.cpp src/main.cpp src/engine.cpp)
target_include_directories(main PUBLIC include/)
