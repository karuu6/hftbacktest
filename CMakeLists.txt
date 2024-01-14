cmake_minimum_required(VERSION 3.10)

project(HFTBacktest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(main src/csv.cpp src/book.cpp src/main.cpp)
target_include_directories(main PUBLIC include/)
