#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"
#include "sudoku.h"
#include <time.h>
#include <math.h>
#include <unistd.h>

// move to bitboard.c
void pprint_bitboard81(Bitboard96 bb)
{
    printf("\n");
    pprint_bitboard96(bb, '*', 0, 81, 9);
    printf("\n");
}

static Bitboard96 cachedRowBitboards[9];
static Bitboard96 cachedColBitboards[9];
static Bitboard96 cachedSubgridBitboards[9];

int get_row_from_index(int i)
{
    return floor(i / 9);
}

int get_col_from_index(int i)
{
    return i % 9;
}

Bitboard96 subgrid_bitboard(int subgrid_index)
{
    return cachedSubgridBitboards[subgrid_index];
}

Bitboard96 row_bitboard(int row_index)
{
    return cachedRowBitboards[row_index];
}
Bitboard96 col_bitboard(int col_index)
{
    return cachedColBitboards[col_index];
}

Bitboard96 _row_bitboard(int row_index)
{
    Bitboard96 bb = newBitboard96();
    for (int i = row_index * 9; i < (row_index + 1) * 9; i++)
    {
        set_bit(&bb, i);
    }
    return bb;
}

Bitboard96 _col_bitboard(int col_index)
{
    Bitboard96 bb = newBitboard96();
    for (int i = col_index; i < 81; i += 9)
    {
        set_bit(&bb, i);
    }
    return bb;
}

int get_subgrid_index(int square_index)
{
    int row = get_row_from_index(square_index);
    int col = get_col_from_index(square_index);
    return (row % 3) * 3 + (col % 3);
}

Bitboard96 _subgrid_bitboard(int subgrid_index)
{
    int starting_row = (subgrid_index / 3) * 3;
    int starting_col = (subgrid_index % 3) * 3;

    Bitboard96 row_merge = newBitboard96();
    Bitboard96 col_merge = newBitboard96();

    for (int i = 0; i < 3; i++)
    {
        row_merge |= _row_bitboard(starting_row + i);
        col_merge |= _col_bitboard(starting_col + i);
    }

    return row_merge & col_merge;
}

void initializeCache()
{
    for (int i = 0; i < 9; i++)
    {
        cachedRowBitboards[i] = _row_bitboard(i);
        cachedColBitboards[i] = _col_bitboard(i);
        cachedSubgridBitboards[i] = _subgrid_bitboard(i);
    }
}

Bitboard96 mask_available_serially(Bitboard96 *bb, Bitboard96 (*entity_bitboard)(int))
{
    Bitboard96 result = 0;
    for (int i = 0; i < 9; i++)
    {
        Bitboard96 _mask = entity_bitboard(i);
        Bitboard96 available = _mask & *bb;
        if (count_ones(available) == 9)
        {
            result |= available;
        }
    }

    return result;
}

Bitboard96 get_available_rows(Bitboard96 *bb)
{
    return mask_available_serially(bb, row_bitboard);
}

Bitboard96 get_available_cols(Bitboard96 *bb)
{
    return mask_available_serially(bb, col_bitboard);
}

Bitboard96 get_available_subgrids(Bitboard96 *bb)
{
    return mask_available_serially(bb, subgrid_bitboard);
}

int is_solved(Sudoku *sudoku)
{
    if ((sudoku->empty) == 0)
    {
        return 1;
    }
    return 0;
}

int can_be_placed(Sudoku *sudoku, int index, int N)
{
    Bitboard96 _available_squares = ~(sudoku->boards[N]);
    Bitboard96 _available_in_row = get_available_rows(&_available_squares);
    Bitboard96 _available_in_col = get_available_cols(&_available_squares);
    Bitboard96 _available_in_subgrid = get_available_subgrids(&_available_squares);
    Bitboard96 _fill_matrix = _available_in_row & _available_in_col & _available_in_subgrid & oneHotBitboard96(index);
    return _fill_matrix != 0;
}

int solve(Sudoku *sudoku)
{
    if (is_solved(sudoku))
    {
        printf("\n\n!!!!!!!!!!!!!! solved!!!\n\n");
        pprint_sudoku(*sudoku);
        return 1;
    }

    int index_of_first_position = index_of_fist_one(sudoku->empty);
    int current_index = index_of_first_position;

    for (int N = 0; N < 9; N++)
    {
        if (can_be_placed(sudoku, current_index, N))
        {
            set_bit(&sudoku->boards[N], current_index);
            clear_bit(&sudoku->empty, current_index);

            int solved = solve(sudoku);
            if (solved)
            {
                return 1;
            }

            clear_bit(&sudoku->boards[N], current_index);
            set_bit(&sudoku->empty, current_index);
        }
    }
    return 0;
}

Sudoku newSudoku()
{
    initializeCache();
    Sudoku sudoku;
    for (int i = 0; i < 9; i++)
    {
        sudoku.boards[i] = newBitboard96();
    }

    sudoku.empty = (__int128_t)0;
    for (int i = 0; i < 81; i++)
    {
        set_bit(&sudoku.empty, i);
    }

    return sudoku;
}

void load_sudoku(char source[], Sudoku *sudoku) {
    for (int i = 0; source[i] != 0; i++)
    {
        int value = (int)source[i] - 48;
        if (value > 0 && value <= 9)
        {
            set_bit(&sudoku->boards[value - 1], i);
            clear_bit(&sudoku->empty, i);
        }
    }
}

void pprint_sudoku(Sudoku sudoku)
{
    printf("\u256D⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯-\u256E\n");
    for (int yy = 0; yy < 9; yy++)
    {
        if (yy % 3 == 0 && yy != 0)
        {
            printf("+⎯⎯⎯⎯⎯⎯⎯⎯⎯+⎯⎯⎯⎯⎯⎯⎯⎯⎯+⎯⎯⎯⎯⎯⎯⎯⎯⎯+\n");
        }

        for (int xx = 0; xx < 9; xx++)
        {
            if (xx % 3 == 0)
            {
                printf("⏐");
            }

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
        printf("|\n");
    }

    printf("\u2570-⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯-\u256F\n");
}

// void pprint_sudoku(Sudoku sudoku)
// {
//     for (int yy = 0; yy < 9; yy++)
//     {
//         for (int xx = 0; xx < 9; xx++)
//         {
//             int is_empty = 1;
//             for (int n = 0; n < 9; n++)
//             {
//                 if (is_bit_set(&sudoku.boards[n], yy * 9 + xx) == 1)
//                 {
//                     printf(" %u ", n + 1);
//                     is_empty = 0;
//                     break;
//                 }
//             }

//             if (is_empty == 1)
//             {
//                 printf(" _ ");
//             }
//         }
//         printf("\n");
//     }
// }
