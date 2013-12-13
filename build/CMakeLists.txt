cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../src)
add_executable(tinylib ${CMAKE_CURRENT_LIST_DIR}/../src/main.cpp)