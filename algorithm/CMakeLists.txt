
set(SRC "src")
include_directories(${SRC})

add_subdirectory(test)
add_subdirectory(src)

add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE cmg)
