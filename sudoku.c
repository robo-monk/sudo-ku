#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"
#include "sudoku.h"
#include <time.h>
#include <math.h>
#include <unistd.h>

// move to bitboard.c
void pprint_bitboard81(Bitboard128 bb)
{
    printf("\n");
    pprint_bitboard128(bb, '*', 0, 81, 9);
    printf("\n");
}

static Bitboard128 cachedRowBitboards[9];
static Bitboard128 cachedColBitboards[9];
static Bitboard128 cachedSubgridBitboards[9];

int get_row_from_index(int i)
{
    return floor(i / 9);
}

int get_col_from_index(int i)
{
    return i % 9;
}

Bitboard128 subgrid_bitboard(int subgrid_index)
{
    return cachedSubgridBitboards[subgrid_index];
}

Bitboard128 row_bitboard(int row_index)
{
    return cachedRowBitboards[row_index];
}
Bitboard128 col_bitboard(int col_index)
{
    return cachedColBitboards[col_index];
}

Bitboard128 _row_bitboard(int row_index)
{
    Bitboard128 bb = newBitboard96();
    for (int i = row_index * 9; i < (row_index + 1) * 9; i++)
    {
        set_bit(&bb, i);
    }
    return bb;
}

Bitboard128 _col_bitboard(int col_index)
{
    Bitboard128 bb = newBitboard96();
    for (int i = col_index; i < 81; i += 9)
    {
        set_bit(&bb, i);
    }
    return bb;
}

int get_subgrid_index(int square_index)
{
    int row = square_index / 9;
    int col = square_index % 9;

    int subgrid_row = row / 3;
    int subgrid_col = col / 3;

    return subgrid_row * 3 + subgrid_col;
}

Bitboard128 _subgrid_bitboard(int subgrid_index)
{
    int starting_row = (subgrid_index / 3) * 3;
    int starting_col = (subgrid_index % 3) * 3;

    Bitboard128 row_merge = newBitboard96();
    Bitboard128 col_merge = newBitboard96();

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

Bitboard128 mask_available_serially(Bitboard128 *bb, Bitboard128 (*entity_bitboard)(int))
{
    Bitboard128 result = 0;
    for (int i = 0; i < 9; i++)
    {
        Bitboard128 _mask = entity_bitboard(i);
        Bitboard128 available = _mask & *bb;
        if (count_ones(available) == 9)
        {
            result |= available;
        }
    }

    return result;
}

Bitboard128 get_available_rows(Bitboard128 *bb)
{
    return mask_available_serially(bb, row_bitboard);
}

Bitboard128 get_available_cols(Bitboard128 *bb)
{
    return mask_available_serially(bb, col_bitboard);
}

Bitboard128 get_available_subgrids(Bitboard128 *bb)
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

void _fill_matrix_for_n(Sudoku *sudoku, int N, Bitboard128 *result)
{
    Bitboard128 _available_squares = ~(sudoku->boards[N]);
    Bitboard128 _available_in_row = get_available_rows(&_available_squares);
    Bitboard128 _available_in_col = get_available_cols(&_available_squares);
    Bitboard128 _available_in_subgrid = get_available_subgrids(&_available_squares);
    *result = _available_in_row & _available_in_col & _available_in_subgrid;
}

void fill_number(Sudoku *sudoku, int N, int index)
{
    set_bit(&sudoku->boards[N], index);
    clear_bit(&sudoku->empty, index);

    int row = get_row_from_index(index);
    int col = get_col_from_index(index);
    int subgrid = get_subgrid_index(index);

    sudoku->fill_matrices[N] &= ~(row_bitboard(row) | col_bitboard(col) | subgrid_bitboard(subgrid));
}

void erase_number(Sudoku *sudoku, int N, int index)
{
    clear_bit(&sudoku->boards[N], index);
    set_bit(&sudoku->empty, index);
}

void bubble_sort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

typedef struct
{
    int index;
    int set_bits;
} Pair;

int compare(const void *a, const void *b)
{
    Pair *pair_a = (Pair *)a;
    Pair *pair_b = (Pair *)b;
    return pair_a->set_bits - pair_b->set_bits;
}

// organizes the bitboards based on the least filled matrices (thus the least possible solutions)
int *get_order_for_ns(Sudoku *sudoku)
{
    // int order[9];
    int *order = (int *)malloc(9 * sizeof(int));

    Pair pairs[9];

    for (int i = 0; i < 9; i++)
    {
        Bitboard128 fill_matrix = sudoku->fill_matrices[i];
        int ones = count_ones(fill_matrix);
        pairs[i].index = i;
        pairs[i].set_bits = ones;
    }

    qsort(pairs, 9, sizeof(Pair), compare);

    for (int i = 0; i < 9; i++)
    {
        order[i] = pairs[i].index;
    }

    return order;
}

int solve(Sudoku *sudoku)
{
    if (is_solved(sudoku))
    {
        return 1;
    }

    int index_of_first_position = index_of_fist_one(sudoku->empty);
    int current_index = index_of_first_position;

    int *order = get_order_for_ns(sudoku);

    for (int i = 0; i < 9; i++)
    {
        int N = order[i];
        Bitboard128 fill_matrix = sudoku->fill_matrices[N];

        if (fill_matrix > 0 && fill_matrix & oneHotBitboard96(current_index))
        {
            Bitboard128 og_fill_matrix = sudoku->fill_matrices[N];
            fill_number(sudoku, N, current_index);

            if (solve(sudoku))
            {

                return 1;
            }

            erase_number(sudoku, N, current_index);
            sudoku->fill_matrices[N] = og_fill_matrix;
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

void load_sudoku(char source[], Sudoku *sudoku)
{
    for (int i = 0; source[i] != 0; i++)
    {
        int value = (int)source[i] - 48;
        if (value > 0 && value <= 9)
        {
            fill_number(sudoku, value - 1, i);
        }
    }

    for (int N = 0; N < 9; N++)
    {
        _fill_matrix_for_n(sudoku, N, &sudoku->fill_matrices[N]);
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
