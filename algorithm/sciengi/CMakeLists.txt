cmake_minimum_required(VERSION 3.30)

project(graphcol VERSION 0.1
                 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(SRC "src")
include_directories(${SRC})


add_subdirectory(test)
add_subdirectory(src)

add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE cmg)
