#include <stdio.h>
// #include "bitboard.h"
#include "sudoku.h"

int main() {
    // Bitboard96 board = newBitboard96();

    // fill_with_noise(&board);
    // pprint_bitboard96(board, '*', 0, 81, 9);

    // // set_bit(&board, 45);

    // for (int i = 0; i < 81; i +=10) {
    //     set_bit(&board, i);
    // }
    // pprint_bitboard96(board, '*', 0, 81, 9);

    Sudoku sudoku = newSudoku();
    pprint_sudoku(sudoku);
}
