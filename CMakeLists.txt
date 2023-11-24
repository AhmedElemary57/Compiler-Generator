cmake_minimum_required(VERSION 3.21)
project(CompilerGenerator)

set(CMAKE_CXX_STANDARD 14)

add_executable(CompilerGenerator main.cpp parser.cpp parser.h)
