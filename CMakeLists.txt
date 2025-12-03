cmake_minimum_required(VERSION 3.25)
project(ParserTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Put executables in the project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(test parser.cpp)
add_executable(inter interactive.cpp)