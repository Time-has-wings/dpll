#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>

#define NEEDRANDOM

struct SUDOKU {
    int sudoku[9][9];
    bool generated;
    int solution_count;

    SUDOKU():generated(false), solution_count(0) {
        memset(sudoku, 0, sizeof(sudoku));
    }

    ~SUDOKU() {}
};

void RANDOM();
SUDOKU createSudoku();
void printSudoku(SUDOKU& sudoku);
void digSudoku(SUDOKU& sudoku, int digCount);

#endif