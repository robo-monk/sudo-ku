#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

typedef struct
{
    uint32_t high;
    uint64_t low;
} Bitboard96;

void set_bit(Bitboard96 *board, int index);
void clear_bit(Bitboard96 *board, int index);
int is_bit_set(Bitboard96 *board, int index);
void pprint_bitboard96(Bitboard96 board, char symbol, int start, int end, int cols);
void fill_with_noise(Bitboard96 *board);
int is_empty(Bitboard96 *board);

Bitboard96 b96_and(Bitboard96 *a, Bitboard96 *b);
void put_b96_and(Bitboard96 *a, Bitboard96 *b, Bitboard96 *result);

Bitboard96 b96_or(Bitboard96 *a, Bitboard96 *b);
void put_b96_or(Bitboard96 *a, Bitboard96 *b, Bitboard96 *result);

Bitboard96 b96_not(Bitboard96 *a);
void put_b96_not(Bitboard96 *a, Bitboard96 *result);





Bitboard96 newBitboard96();
Bitboard96 oneHotBitboard96(int hot_index);

#endif /* BITBOARD_H */
