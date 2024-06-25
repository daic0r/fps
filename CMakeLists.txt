cmake_minimum_required(VERSION 3.21)

project(FPS)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_BUILD_TYPE Debug)
   
add_subdirectory(src)

target_link_libraries(FPS PRIVATE raylib)
target_precompile_headers(FPS PRIVATE src/precompiled.h)

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
   src/test/plane_test.cpp
   src/test/line_test.cpp
   src/test/triangle_test.cpp
)
target_link_libraries(
   tests
   GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
