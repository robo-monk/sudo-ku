#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"
#include <time.h>

Bitboard96 newBitboard96()
{
    Bitboard96 board = 0;
    return board;
}

Bitboard96 oneHotBitboard96(int hot_index)
{
    return (Bitboard96)1 << hot_index;
}

int is_empty(Bitboard96 *bb)
{
    return bb == 0;
}

int lowest_set_bit(__int128 num)
{
    if (num == 0)
    {
        return -1; // No set bits
    }

    int position = 0;
    uint64_t highPart = (uint64_t)(num >> 64); // Extract upper 64 bits
    uint64_t lowPart = (uint64_t)num;          // Extract lower 64 bits

    if (lowPart != 0)
    {
        position = __builtin_ctzll(lowPart);
    }
    else
    {
        position = 64 + __builtin_ctzll(highPart);
    }

    return position;
}

int highest_set_bit(__int128 num)
{
    if (num == 0)
    {
        return -1; // No set bits
    }

    int position = 0;
    uint64_t highPart = (uint64_t)(num >> 64); // Extract upper 64 bits
    uint64_t lowPart = (uint64_t)num;          // Extract lower 64 bits

    if (highPart != 0)
    {
        position = 64 + __builtin_clzll(highPart);
    }
    else
    {
        position = __builtin_clzll(lowPart);
    }

    return 127 - position;
}

int index_of_fist_one(Bitboard96 bb)
{
    return lowest_set_bit(bb);
}

void set_bit(Bitboard96 *board, int index)
{
    *board |= (__int128_t)1 << index;
}

void clear_bit(Bitboard96 *board, int index)
{

    *board &= ~((__int128_t)1 << index);
}

int is_bit_set(Bitboard96 *board, int index)
{
    return (*board & ((__int128_t)1 << index)) != 0;
}

void fill_with_noise(Bitboard96 *board)
{
    // TODO implement
    board = (__int128_t)rand() << 32 | rand();
}

int count_ones(Bitboard96 num)
{
    int count = 0;

    while (num > 0)
    {
        count += __builtin_popcountll((uint64_t)num);
        num >>= 64;
    }

    return count;
}

void pprint_bitboard96(Bitboard96 board, char symbol, int start, int end, int cols)
{
    int row_index = 0;
    for (int i = start; i < end; i++)
    {
        if (is_bit_set(&board, i) == 1)
        {
            printf(" %c ", symbol);
        }
        else
        {
            printf(" _ ");
        }

        if (row_index == cols - 1)
        {
            printf("\n");
            row_index = 0;
        }
        else
        {
            row_index += 1;
        }
    }
}
