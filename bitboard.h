#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

typedef __int128_t Bitboard96;

void set_bit(Bitboard96 *board, int index);
void clear_bit(Bitboard96 *board, int index);
int is_bit_set(Bitboard96 *board, int index);
void pprint_bitboard96(Bitboard96 board, char symbol, int start, int end, int cols);
void fill_with_noise(Bitboard96 *board);
int is_empty(Bitboard96 *board);
int index_of_fist_one(Bitboard96 bb);
int count_ones(Bitboard96 num);

Bitboard96 newBitboard96();
Bitboard96 oneHotBitboard96(int hot_index);

#endif /* BITBOARD_H */
