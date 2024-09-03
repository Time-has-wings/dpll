#include "sudoku.h"

void RANDOM() {
#ifdef NEEDRANDOM
    srand(time(0));
#endif
}

static void myShuffle(int* arr, int count) {
    for (int i = 0; i < count; i++) {
        int idx = rand() % count;
        std::swap(arr[i], arr[idx]);
    }
}

static int belongGrid(int x, int y) {
    return x / 3 * 3 + y / 3;
}

static void availableNumber(int x, int y, int* arr, int& count, bool row[][9], bool col[][9], bool grid[][9], bool diag[]) {
    int id = belongGrid(x, y);
    count = 0;
    for (int i = 1; i <= 9; i++) {
        if (row[x][i - 1] || col[y][i - 1] || grid[id][i - 1] || ((x == y) && diag[i - 1])) 
            continue;
        arr[count++] = i;
    }
}

static void setNumber(SUDOKU& sudoku, int x, int y, int num, bool row[][9], bool col[][9], bool grid[][9], bool diag[]) {
    sudoku.sudoku[x][y] = num;
    row[x][num - 1] = true;
    col[y][num - 1] = true;
    grid[belongGrid(x, y)][num - 1] = true;
    if (x == y) diag[num - 1] = true;
}

static void resetNumber(SUDOKU& sudoku, int x, int y, int num, bool row[][9], bool col[][9], bool grid[][9], bool diag[]) {
    sudoku.sudoku[x][y] = 0;
    row[x][num - 1] = false;
    col[y][num - 1] = false;
    grid[belongGrid(x, y)][num - 1] = false;
    if (x == y) diag[num - 1] = false;
}

void printSudoku(SUDOKU& sudoku) {
    std::cout << "==========================================" << std::endl;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << sudoku.sudoku[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

static void generateSudoku(SUDOKU& sudoku, int x, int y, bool row[][9], bool col[][9], bool grid[][9], bool diag[]) {
    if (sudoku.generated == true) return;
    if (x == 9) sudoku.generated = true;
    else if (y == 9) generateSudoku(sudoku, x + 1, 0, row, col, grid, diag);
    else {
        int arr[9], count;
        availableNumber(x, y, arr, count, row, col, grid, diag);
        myShuffle(arr, count);
        for (int i = 0; i < count; i++) {
            setNumber(sudoku, x, y, arr[i], row, col, grid, diag);
            generateSudoku(sudoku, x, y + 1, row, col, grid, diag);
            if (sudoku.generated == true) return;
            resetNumber(sudoku, x, y, arr[i], row, col, grid, diag);
        }
    }
}

SUDOKU createSudoku() {
    SUDOKU sudoku{};
    bool row[9][9], col[9][9], grid[9][9], diag[9];
    memset(row, false, sizeof(row));
    memset(col, false, sizeof(col));
    memset(grid, false, sizeof(grid));
    memset(diag, false, sizeof(diag));
    generateSudoku(sudoku, 0, 0, row, col, grid, diag);
    return sudoku;
}

static void solveSudoku(SUDOKU& sudoku, int x, int y, bool row[][9], bool col[][9], bool grid[][9], bool diag[]) {
    if (x == 9) sudoku.solution_count++;
    else if (y == 9) solveSudoku(sudoku, x + 1, 0, row, col, grid, diag);
    else if (sudoku.sudoku[x][y] != 0) solveSudoku(sudoku, x, y + 1, row, col, grid, diag);
    else {
        int arr[9], count;
        availableNumber(x, y, arr, count, row, col, grid, diag);
        for (int i = 0; i < count; i++) {
            setNumber(sudoku, x, y, arr[i], row, col, grid, diag);
            solveSudoku(sudoku, x, y + 1, row, col, grid, diag);
            resetNumber(sudoku, x, y, arr[i], row, col, grid, diag);
        }
    }
}

void digSudoku(SUDOKU& sudoku, int digCount) {
    bool row[9][9], col[9][9], grid[9][9], diag[9];
    memset(row, true, sizeof(row));
    memset(col, true, sizeof(col));
    memset(grid, true, sizeof(grid));
    memset(diag, true, sizeof(diag));
    for (int i = 0; i < digCount; i++) {
        while (true) {
            int x = rand() % 9, y = rand() % 9;
            int backup = sudoku.sudoku[x][y];
            if (backup == 0) continue;
            resetNumber(sudoku, x, y, backup, row, col, grid, diag);
            sudoku.solution_count = 0;
            solveSudoku(sudoku, 0, 0, row, col, grid, diag);
            if (sudoku.solution_count == 1) break;
            else setNumber(sudoku, x, y, backup, row, col, grid, diag);
        }   
    }
}