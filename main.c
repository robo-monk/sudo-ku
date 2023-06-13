#include <stdio.h>
#include "bitboard.h"

int main() {
    Bitboard96 board = newBitboard96();

    // fill_with_noise(&board);
    pprint(board, '*', 0, 81, 9);

    // set_bit(&board, 45);

    for (int i = 0; i < 81; i +=10) {
        set_bit(&board, i);
    }
    pprint(board, '*', 0, 81, 9);
}
