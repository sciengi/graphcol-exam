cmake_minimum_required(VERSION 3.30)

project(graphcol VERSION 0.1
                 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(SRC         "src")
set(M_MATRIX    "${SRC}/matrix")
set(M_COLORING  "${SRC}/coloring")

include_directories(${SRC} ${M_MATRIX} ${M_COLORING})


add_subdirectory(test)
add_subdirectory(src)

add_executable(${CMAKE_PROJECT_NAME} demo.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE cmg)
