#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdint.h>
#include "bitboard.h"

typedef struct
{
    Bitboard96 boards[9];
    Bitboard96 empty;
} Sudoku;

int solve(Sudoku *sudoku);
void pprint_sudoku(Sudoku sudoku);
void load_sudoku(char source[], Sudoku *sudoku);
Sudoku newSudoku();

#endif /* SUDOKU_H */
