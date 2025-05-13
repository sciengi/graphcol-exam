
find_package(GTest)

if (GTest_FOUND)
    file(GLOB SOURCES "*.cpp")
    add_executable(tester ${SOURCES})
    target_link_libraries(tester PRIVATE cmg gtest gtest_main)
    target_compile_features(tester PRIVATE cxx_std_17)
else()
    message(STATUS "gtest not found - not building tests")
endif()

