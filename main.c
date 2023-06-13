#include <stdio.h>
#include "bitboard.h"

int main() {
    Bitboard96 board = newBitboard96();

    fill_with_noise(&board);
    pprint(board, '*', 0, 81, 9);
}
