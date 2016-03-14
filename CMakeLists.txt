cmake_minimum_required(VERSION 3.3)
project(scaffold)

set(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -std=c++14 -Wall")

file(GLOB_RECURSE sources 
    src/*.cc 
    src/*.hh 
    main/*.cc 
    main/*.hh)

add_executable(scaffold ${sources})
