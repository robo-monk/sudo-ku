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
static Bitboard96 cachedSubsquareBitboards[9];

int get_row_from_index(int i)
{
    return floor(i / 9);
}

int get_col_from_index(int i)
{
    return i % 9;
}

Bitboard96 subsquare_bitboard(int subsquare_index) {
    return cachedSubsquareBitboards[subsquare_index];
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

int get_subsquare_index(int square_index) {
    int row = get_row_from_index(square_index);
    int col = get_col_from_index(square_index);
    return (row % 3) * 3 + (col % 3);
}

Bitboard96 _subsquare_bitboard(int subsquare_index) {
    int starting_row = (subsquare_index / 3) * 3;
    int starting_col = (subsquare_index % 3) * 3;

    Bitboard96 row_merge = newBitboard96();
    Bitboard96 col_merge = newBitboard96();

    for (int i = 0; i < 3; i++) {
        row_merge |= _row_bitboard(starting_row+i);
        col_merge |= _col_bitboard(starting_col+i);
    }

    return row_merge & col_merge;
}

void initializeCache()
{
    for (int i = 0; i < 9; i++)
    {
        cachedRowBitboards[i] = _row_bitboard(i);
        cachedColBitboards[i] = _col_bitboard(i);
        cachedSubsquareBitboards[i] = _subsquare_bitboard(i);
        // printf("- subgrid: %u -\n", i);
        // pprint_bitboard81(_subsquare_bitboard(i));
    }
}



// TODO: move to bitboard.c
int count_ones(__int128 num)
{
    int count = 0;

    while (num > 0)
    {
        count += __builtin_popcountll((uint64_t)num);
        num >>= 64;
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

Bitboard96 get_available_subsquares(Bitboard96 *bb) {
    Bitboard96 result = 0;
    for (int i = 0; i < 9; i++)
    {

        // printf("for col %u \n", i);
        Bitboard96 _subsquare_bb = subsquare_bitboard(i);
        Bitboard96 available = _subsquare_bb & *bb;

        // pprint_bitboard81(available);
        if (count_ones(available) == 9)
        {
            result |= available;
        }
    }

    return result;

}

int countOnes(__int128 value)
{
    int count = 0;

    // Iterate over each bit
    for (int i = 0; i < 128; i++)
    {
        // Check if the bit is set
        if ((value & ((__int128)1 << i)) != 0)
        {
            count++;
        }
    }

    return count;
}

int is_solved(Sudoku *sudoku)
{
    // printf("is solved?");
    // pprint_bitboard81(sudoku->empty);
    // printf("size of %zu", 8 * sizeof(sudoku->empty));
    // printf("size of %zu", 8 * sizeof((__int128_t)0));
    // printf("count ones in empty: %u", countOnes(sudoku->empty));
    // exit(0);
    if ((sudoku->empty) == 0)
    {
        printf("!!! SOLVED!");
        return 1;
    }
    return 0;
}

Sudoku *_solve(Sudoku *sudoku)
{
    // printf("-<");
    // sleep(1);

    if (is_solved(sudoku))
    {
        printf("solved!!!");
        pprint_sudoku(*sudoku);
        return sudoku;
    }

    for (int i = 0; i < 9; i++)
    {
        // printf("for %u \n", i);
        Bitboard96 _available_squares = ~(sudoku->boards[i]);
        // pprint_bitboard81(_available_squares);
        // printf("empties: \n");
        // pprint_bitboard81(sudoku->empty);
        Bitboard96 _available_in_row = get_available_rows(&_available_squares);
        // printf("row mask: \n");
        // pprint_bitboard81(_available_in_row);

        // pprint_bitboard81(_available_in_row);
        Bitboard96 _available_in_col = get_available_cols(&_available_squares);
        // printf("col mask: \n");
        // pprint_bitboard81(_available_in_col);

        // pprint_bitboard81(_available_in_col);
        Bitboard96 _fill_matrix = _available_in_row & _available_in_col & sudoku->empty;
        Bitboard96 _og_fill_matrix = _fill_matrix;

        if (_fill_matrix == 0)
            return NULL;
        // printf("fill matrix: \n");

        // pprint_bitboard96(_fill_matrix, '*', 0, 96, 8);
        pprint_bitboard81(_fill_matrix);
        int index_of_first_position = index_of_fist_one(_fill_matrix);
        printf("INDEX OF FIRST ONE: %u\n", index_of_first_position);
        int current_index = index_of_first_position;

        while (_fill_matrix != 0 && current_index > 0 && current_index < 81)
        {
            printf("-> go in \n");
            // printf("_fill_matrix is valid! %u", i);
            // printf("current index: %u", current_index);
            // pprint_bitboard81(_fill_matrix);
            // if (index_of_first_position > 81)
            // {
            //     printf("this should not happen (i: %u) \n", i);
            //     printf("empties: \n");
            //     pprint_bitboard81(sudoku->empty);

            //     printf("col mask: \n");
            //     pprint_bitboard81(_available_in_col);

            //     printf("fill matrix: \n");
            //     pprint_bitboard81(_fill_matrix);

            //     pprint_sudoku(*sudoku);
            //     exit(1);
            //     // return NULL;
            // }

            // printf("i: %u", index_of_first_position);
            Sudoku temp_sudoku = *sudoku;

            // printf("index of first one: %u \n", index_of_first_position);

            Bitboard96 fill = oneHotBitboard96(current_index) & temp_sudoku.empty;

            pprint_sudoku(temp_sudoku);
            printf("[%u: i] fill! (%u) current-index \n", i, current_index);
            printf("-> fill is\n");
            pprint_bitboard81(fill);
            // pprint_bitboard81(_og_fill_matrix);
            // pprint_bitboard81(_fill_matrix);
            // pprint_sudoku(temp_sudoku);
            // if (current_index == 63) {
            //     exit(1);
            // }

            // printf("fill matrix: \n");
            // pprint_bitboard81(fill);
            // Bitboard96 _fill = fill | temp_sudoku.boards[i];
            temp_sudoku.boards[i] |= fill;
            temp_sudoku.empty &= ~fill;

            printf("temp sudoku now: \n");
            pprint_sudoku(temp_sudoku);

            // _fill_matrix >>= (index_of_first_position + 1);
            _fill_matrix >>= (index_of_first_position + 1);
            // pprint_bitboard81(_fill_matrix);
            index_of_first_position = index_of_fist_one(_fill_matrix);
            current_index += index_of_first_position;
            Sudoku *result = solve(&temp_sudoku);
            if (result != NULL)
            {
                return result;
            }
            // printf("[%u] next -> \nfill! current-index: %u\n (index of first position: %u) (_fill_matrix: %llu%016llu)", i, current_index, index_of_first_position, _fill_matrix >> 64, _fill_matrix);
            // pprint_bitboard81(_og_fill_matrix);
        };
    }

    printf("\n");
    pprint_sudoku(*sudoku);
    printf("\n");

    // printf(">-");
    return NULL;
}

int can_be_placed(Sudoku *sudoku, int index, int N)
{
    Bitboard96 _available_squares = ~(sudoku->boards[N]);
    // pprint_bitboard81(_available_squares);
    // printf("empties: \n");
    // pprint_bitboard81(sudoku->empty);
    Bitboard96 _available_in_row = get_available_rows(&_available_squares);
    // printf("row mask: \n");
    // pprint_bitboard81(_available_in_row);

    // pprint_bitboard81(_available_in_row);
    Bitboard96 _available_in_col = get_available_cols(&_available_squares);
    // printf("col mask: \n");
    // pprint_bitboard81(_available_in_col);

    Bitboard96 _available_in_subsquare = get_available_subsquares(&_available_squares);

    // pprint_bitboard81(_available_in_col);
    // Bitboard96 _fill_matrix = _available_in_row & _available_in_col & sudoku->empty;
    Bitboard96 _fill_matrix = _available_in_row & _available_in_col & _available_in_subsquare & oneHotBitboard96(index);

    return _fill_matrix != 0;
}

int solve(Sudoku *sudoku)
{
    // pprint_sudoku(*sudoku);
    // printf("-<");
    // sleep(100);
    // usleep(5*1000);

    // pprint_sudoku(*sudoku);

    if (is_solved(sudoku))
    {
        printf("\n\n!!!!!!!!!!!!!! solved!!!\n\n");
        pprint_sudoku(*sudoku);
        // return sudoku;
        return 1;
    }

    // Bitboard96 empty_sq = sudoku->empty;

    int index_of_first_position = index_of_fist_one(sudoku->empty);
    // printf("INDEX OF FIRST POS: %u\n", index_of_first_position);

    int current_index = index_of_first_position;
    // int current_index = index_of_first_position;
    // while (~(sudoku->empty) > 0 && current_index >= 0 && current_index < 81)
    // {
    // printf("-> current index: %u \n", current_index);
    // pprint_bitboard81(empty_sq);
    // printf("-> go in \n");
    // printf("_fill_matrix is valid! %u", i);
    // printf("current index: %u", current_index);
    // pprint_bitboard81(_fill_matrix);
    // printf("i: %u", index_of_first_position);
    // Sudoku temp_sudoku = *sudoku;

    // printf("index of first one: %u \n", index_of_first_position);

    // Bitboard96 fill = oneHotBitboard96(current_index);
    // for (int _i = 0; _i < 81; _i++)
    // {
    //     int current_index = _i;
    //     if (is_bit_set(&sudoku->empty, _i))
    // {
    for (int N = 0; N < 9; N++)
    {
        if (can_be_placed(sudoku, current_index, N))
        {
            // place it
            // printf("placing [%u] to [%u]... \n", N + 1, current_index);
            set_bit(&sudoku->boards[N], current_index);
            clear_bit(&sudoku->empty, current_index);

            // Sudoku temp_sudoku = *sudoku;
            // int solved = solve(&temp_sudoku);
            int solved = solve(sudoku);
            // *sudoku = temp_sudoku;
            if (solved)
            {
                // printf("im in here");
                // pprint_sudoku(*result);
                return 1;
            }

            clear_bit(&sudoku->boards[N], current_index);
            set_bit(&sudoku->empty, current_index);
        }
    }
    return 0;
    // }
    // }
    return 0;

    // empty_sq >>= (index_of_first_position + 1);

    // empty_sq <<= 1;

    // index_of_first_position = index_of_fist_one(sudoku->empty >> (current_index + 1));
    // printf("indexoffirstpos %u", index_of_first_position);
    // current_index += index_of_first_position + 1;
    // return NULL;

    // pprint_bitboard81(_fill_matrix);

    // printf("[%u] next -> \nfill! current-index: %u\n (index of first position: %u) (_fill_matrix: %llu%016llu)", i, current_index, index_of_first_position, _fill_matrix >> 64, _fill_matrix);
    // pprint_bitboard81(_og_fill_matrix);
    // };
    // printf("-> NO LUCK");
    // pprint_sudoku(*sudoku);
    // return sudoku;
    // return NULL;
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

    // printf("empty bit: ");
    // pprint_bitboard81(sudoku.empty);
    // printf("\n");
}
