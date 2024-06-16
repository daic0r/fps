cmake_minimum_required(VERSION 3.21)

project(FPS)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
   
add_subdirectory(src)

target_link_libraries(FPS PRIVATE raylib)

include(FetchContent)
FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
   tests
   src/test/vector_test.cpp
   src/test/matrix_test.cpp
)
target_link_libraries(
   tests
   GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
