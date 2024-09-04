cmake_minimum_required(VERSION 3.5)
project(sudoku)

include_directories(include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB SRC_FILES "src/*.cpp")

add_executable(main main.cpp ${SRC_FILES})
add_executable(sudoku-test test/sudoku-test.cpp ${SRC_FILES})
add_executable(dpll-test test/dpll-test.cpp ${SRC_FILES} )