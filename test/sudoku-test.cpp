#include "sudoku.h"

using namespace std;

int main() {
    RANDOM();
    SUDOKU sudoku = createSudoku();
    printSudoku(sudoku);
    digSudoku(sudoku, 20);
    printSudoku(sudoku);
    return 0;
}