#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

typedef __int128_t Bitboard128;

void set_bit(Bitboard128 *board, int index);
void clear_bit(Bitboard128 *board, int index);
int is_bit_set(Bitboard128 *board, int index);
void pprint_bitboard128(Bitboard128 board, char symbol, int start, int end, int cols);
void fill_with_noise(Bitboard128 *board);
int is_empty(Bitboard128 *board);
int index_of_fist_one(Bitboard128 bb);
int count_ones(Bitboard128 num);

Bitboard128 newBitboard96();
Bitboard128 oneHotBitboard96(int hot_index);

#endif /* BITBOARD_H */
