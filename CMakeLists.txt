cmake_minimum_required(VERSION 3.12)
project(experimental)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(OpenSSL)

add_executable(experimental main.cpp)

target_link_libraries(experimental curl)