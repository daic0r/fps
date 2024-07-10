cmake_minimum_required(VERSION 3.21)

project(FPS)

option(ASAN "Use address sanitizer" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-p=${CMAKE_CURRENT_BINARY_DIR};-checks=-*,performance-*,modernize-*,readability-*;")

if (ASAN)
   string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")
   string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address")
endif()
   
find_package(SDL2 REQUIRED)
#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARY NAME SDL2)

add_subdirectory(src)

target_include_directories(FPS PRIVATE ${SDL2_INCLUDE_DIR} src)
message(STATUS "SDL2_INCLUDE_DIR: ${SDL2_INCLUDE_DIR}")
target_link_libraries(FPS PRIVATE raylib SDL2::SDL2main SDL2::SDL2 SDL2::SDL2-static)
target_precompile_headers(FPS PRIVATE src/precompiled.h)

include(FetchContent)
FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Silence clang-tidy warnings from googletest
set_target_properties(gtest PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gtest_main PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock PROPERTIES CXX_CLANG_TIDY "")
set_target_properties(gmock_main PROPERTIES CXX_CLANG_TIDY "")

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
target_include_directories(
   tests
   PRIVATE
   src
)

include(GoogleTest)
gtest_discover_tests(tests)
