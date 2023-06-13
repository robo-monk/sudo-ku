#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

typedef struct {
    uint32_t high;
    uint64_t low;
} Bitboard96;

void set_bit(Bitboard96* board, int index);
void clear_bit(Bitboard96* board, int index);
int is_bit_set(Bitboard96* board, int index);
void pprint(Bitboard96 board, char symbol, int start, int end, int cols);
void fill_with_noise(Bitboard96* board);
Bitboard96 newBitboard96();



#endif /* BITBOARD_H */
