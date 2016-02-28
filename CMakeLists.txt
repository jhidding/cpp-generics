cmake_minimum_required(VERSION 3.3)
project(spirit_json)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB_RECURSE sources src/*.cc src/*.hh main/*.cc main/*.hh)

add_executable(spirit_json ${sources})
