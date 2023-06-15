#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"
#include "sudoku.h"
#include <time.h>
#include <math.h>

void pprint_bitboard81(Bitboard96 bb)
{
    printf("\n");
    pprint_bitboard96(bb, '*', 0, 81, 9);
    printf("\n");
}

Bitboard96 row_bitboard(int row_index)
{
    Bitboard96 bb = newBitboard96();
    for (int i = row_index * 9; i < (row_index + 1) * 9; i++)
    {
        set_bit(&bb, i);
    }
    return bb;
}

Bitboard96 col_bitboard(int col_index)
{
    Bitboard96 bb = newBitboard96();
    for (int i = col_index; i < 81; i += 9)
    {
        set_bit(&bb, i);
    }
    return bb;
}

int get_row_from_index(int i)
{
    return floor(i / 9);
}

int get_col_from_index(int i)
{
    return i % 9;
}

int count_zeros(Bitboard96 num)
{
    int count = 0;
    __int128 mask = 1;

    while (num != 0)
    {
        if ((num & mask) == 0)
        {
            count++;
        }
        num >>= 1;
    }

    return count;
}

int count_ones(__int128 num)
{
    int count = 0;
    __int128 mask = 1;

    while (num != 0)
    {
        if ((num & mask) != 0)
        {
            count++;
        }
        num >>= 1;
    }

    return count;
}

Bitboard96 get_available_rows(Bitboard96 *bb)
{
    Bitboard96 result = 0;
    for (int i = 0; i < 9; i++)
    {
        Bitboard96 _row_bb = row_bitboard(i);

        // printf("for row %u \n", i);
        // pprint_bitboard81(_row_bb);
        Bitboard96 available = _row_bb & *bb;
        if (count_ones(available) == 9)
        {
            // printf("available to place at row %u ()", 8);
            result |= available;
        }
        // printf("zeors %u", count_ones(available));
        // pprint_bitboard81(available);
    }

    return result;
}

Bitboard96 get_available_cols(Bitboard96 *bb)
{
    Bitboard96 result = 0;
    for (int i = 0; i < 9; i++)
    {

        // printf("for col %u \n", i);
        Bitboard96 _col_bb = col_bitboard(i);
        Bitboard96 available = _col_bb & *bb;

        // pprint_bitboard81(available);
        if (count_ones(available) == 9)
        {
            result |= available;
        }
    }

    return result;
}

void shuffle(Sudoku *sudoku)
{
    // for now lets just do a random invalid sudoku state
    for (int i = 0; i < 9; i++)
    {
        // u_int8_t hot_index = rand() % 10;
        // for (int hot_index = 0; hot_index < 12; hot_index++)
        // {
        // Bitboard96 hot_bb = oneHotBitboard96(hot_index);

        // int row = get_row_from_index(hot_index);
        // int col = get_col_from_index(hot_index);
        // Bitboard96 _row_bb = row_bitboard(row);
        // Bitboard96 _col_bb = col_bitboard(col);

        // Bitboard96 _is_occuppied_bb = b96_and(&hot_bb, &sudoku->empty);

        printf("for %u \n", i);
        // Bitboard96 _not_squares = b96_not(&sudoku->boards[i]);
        Bitboard96 _available_squares = ~(sudoku->boards[i]);
        // pprint_bitboard81(_available_squares);
        // printf("empties: \n");
        // pprint_bitboard81(sudoku->empty);
        Bitboard96 _available_in_row = get_available_rows(&_available_squares);
        printf("row mask: \n");
        pprint_bitboard81(_available_in_row);

        // pprint_bitboard81(_available_in_row);
        Bitboard96 _available_in_col = get_available_cols(&_available_squares);
        printf("col mask: \n");
        pprint_bitboard81(_available_in_col);


        // pprint_bitboard81(_available_in_col);
        Bitboard96 _fill_matrix = _available_in_row & _available_in_col & sudoku->empty;
        printf("fill matrix: \n");
        pprint_bitboard81(_fill_matrix);

        // pprint_bitboard96(_fill_matrix, '*', 0, 96, 8);
        int index_of_first_position = index_of_fist_one(_fill_matrix);
        printf("index of first one: %u \n", index_of_first_position);

        Bitboard96 fill = oneHotBitboard96(index_of_first_position);

        printf("fill matrix: \n");
        pprint_bitboard81(fill);
        Bitboard96 _fill = fill | sudoku->boards[i];
        // put_b96_and(&_fill, &sudoku->empty, &sudoku->boards[i]);
        sudoku->boards[i] |= _fill;
        sudoku->empty &= ~_fill;
        // sudoku->empty = ~_fill;

        // fill_with_noise(&sudoku->boards[i]);
        // pprint_bitboard96(sudoku->boards[i], '*', 0, 81, 9);
        // printf("\n");
    }

    Bitboard96 empty = newBitboard96();
    Bitboard96 _row_bb = col_bitboard(0);
    // put_b96_and(&empty, &_row_bb, &sudoku->boards[0]);
}

Sudoku newSudoku()
{
    Sudoku sudoku;
    for (int i = 0; i < 9; i++)
    {
        sudoku.boards[i] = newBitboard96();
    }

    sudoku.empty = ~0;
    for (int i = 0; i < 9; i++)
    {
        set_bit(&sudoku.boards[i], i * 9 + i);
        clear_bit(&sudoku.empty, i * 9 + i);
    }
    shuffle(&sudoku);
    return sudoku;
}

/*

    fill sq x with one:
        1) square must not be occupied
            (bb[0] | bb[1] | bb[2])[sq] ... === 0)
        2) row_bitboard(row_index) & one_bitboard === 0
        2) col_bitboard(row_index) & col_bitboard === 0

*/

// Bitboard96 ROW_BITBOARDS[] = {
//     {
//         high : 0b11111111100000000000000000000000,
//         low : 0
//     },
//     {
//         high : 0b11111111100000000000000,
//         low : 0
//     },
//     {
//         high : 0b11111111100000,
//         low : 0
//     },
//     {
//         high : 0b11111,
//         low : 0b1111000000000000000000000000000000000000000000000000000000000000
//     }
// }

//...

/*


0000 0000 0000 0000
0000 0000 0001 1111
1111
000 000 000
000 000 000

000 000 000
000 000 000
000 000 000
*/

void shuffle_board(Bitboard96 *board)
{
}

void solve(Sudoku *sudoku) {}
void pprint_sudoku(Sudoku sudoku)
{
    for (int yy = 0; yy < 9; yy++)
    {
        for (int xx = 0; xx < 9; xx++)
        {
            int is_empty = 1;
            for (int n = 0; n < 9; n++)
            {
                if (is_bit_set(&sudoku.boards[n], yy * 9 + xx) == 1)
                {
                    printf(" %u ", n + 1);
                    is_empty = 0;
                    break;
                }
            }

            if (is_empty == 1)
            {
                printf(" _ ");
            }
        }
        printf("\n");
    }
}
