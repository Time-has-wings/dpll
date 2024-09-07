cmake_minimum_required(VERSION 3.5)
project(sudoku)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
file(GLOB SRC_FILES "src/*.cpp")

add_executable(main main.cpp ${SRC_FILES})
add_executable(sudoku-test test/sudoku-test.cpp ${SRC_FILES})
add_executable(dpll-test test/dpll-test.cpp ${SRC_FILES} )
add_executable(dpll-optimize-test test/dpll-optimize-test.cpp ${SRC_FILES})