cmake_minimum_required(VERSION 3.5)
project(sudoku)

include_directories(include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB SRC_FILES "src/*.cpp")

add_executable(main main.cpp ${SRC_FILES})
add_executable(tests test/tests.cpp src/sudoku.cpp include/sudoku.h)
