#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bitboard.h"

Bitboard96 newBitboard96() {
    Bitboard96 board;
    board.high = 0;
    board.low = 0;
    return board;
}

void set_bit(Bitboard96* board, int index) {
    if (index < 64) {
        board->low |= (1ULL << index);
    } else {
        board->high |= (1U << (index - 64));
    }
}

void clear_bit(Bitboard96* board, int index) {
    if (index < 64) {
        board->low &= ~(1ULL << index);
    } else {
        board->high &= ~(1U << (index - 64));
    }
}

int is_bit_set(Bitboard96* board, int index) {
    if (index < 64) {
        return (board->low & (1ULL << index)) != 0;
    } else {
        return (board->high & (1U << index)) != 0;
    }
}

void fill_with_noise(Bitboard96* board) {
    board->low &= rand();
    board->high &= rand();
}


void pprint(Bitboard96 board, char symbol, int start, int end, int cols) {
    int row_index = 0;
    for (int i = start; i <= end; i ++) {
        if (is_bit_set(&board, i) == 1) {
            printf(" %c ", symbol);
        } else {
            printf(" _ ");
        }

        if (row_index == cols-1) {
            printf("\n");
            row_index = 0;
        } else {
            row_index += 1;
        }
    }
}
