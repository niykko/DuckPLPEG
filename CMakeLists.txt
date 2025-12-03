cmake_minimum_required(VERSION 3.25)
project(ParserTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test parser.cpp)