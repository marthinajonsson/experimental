cmake_minimum_required(VERSION 3.12)
project(experimental)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.69.0 COMPONENTS thread chrono filesystem)
find_package(OpenSSL)


if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()


add_executable(experimental main.cpp)

target_link_libraries(experimental curl)