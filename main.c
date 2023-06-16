#include <stdio.h>
// #include "bitboard.h"
#include "sudoku.h"
#include <string.h>

int main()
{
    // Bitboard96 board = newBitboard96();

    // fill_with_noise(&board);
    // pprint_bitboard96(board, '*', 0, 81, 9);

    // // set_bit(&board, 45);

    // for (int i = 0; i < 81; i +=10) {
    //     set_bit(&board, i);
    // }
    // pprint_bitboard96(board, '*', 0, 81, 9);

    Sudoku sudoku = newSudoku();

    char source[] = "800000000003600000070090200050007000000045700000100030001000068008500010090000400";
    // char source[] = "300000000500000700007029501200005080900860100850000043000047050692351070705000019";
    int len = strlen(source);
    for (int i = 0; source[i] != 0; i++)
    {
        // break;
        // printf("%c", source[i]);
        int value = (int)source[i] - 48;
        if (value > 0) {
            set_bit(&sudoku.boards[value-1], i);
            clear_bit(&sudoku.empty, i);
        } 
        // printf("%u\n", );
        // set_bit(&sudoku.boards[i], i * 9 + i);
        // clear_bit(&sudoku.empty, i * 9 + i);
    }

    pprint_sudoku(sudoku);
    // exit(1);
    // pprint_bitboard96(sudoku.empty, 'X', 0, 81, 9);

    solve(&sudoku);

    printf("--> solve return: \n\n");
    pprint_sudoku(sudoku);
}
