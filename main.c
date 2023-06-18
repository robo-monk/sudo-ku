#include <stdio.h>
// #include "bitboard.h"
#include "sudoku.h"
#include <sys/time.h>

long long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}

int main()
{
    // Bitboard96 board = newBitboard96();

    // fill_with_noise(&board);
    // pprint_bitboard96(board, '*', 0, 81, 9);

    // // set_bit(&board, 45);

    // for (int i = 0; i < 81; i +=10) {
    //     set_bit(&board, i);
    // }
    // pprint_bitboard96(board, '*', 0, 81, 9);

    Sudoku sudoku = newSudoku();

    // char source[] = "800000000003600000070090200050007000000045700000100000001000068000500010090000400";
    char source[] = "000000002001000700030050090000006040003040800040509000090060030002000100700003000";
    // char source[] = "5...8..49...5...3..673....115..........2.8..........187....415..3...2...49..5...3";
    // char source[] = "...4....7.98..7...7.3128....15.....82.......68.....53....5648.1...9..47.5....1...";

    // char source[] = "300000000500000700007029501200005080900860100850000043000047050692351070705000019";
    
    load_sudoku(source, &sudoku);

    // return 1;
    pprint_sudoku(sudoku);
    // exit(1);
    // pprint_bitboard96(sudoku.empty, 'X', 0, 81, 9);

    long long start_time = current_timestamp();
    solve(&sudoku);

    long long end_time = current_timestamp();
    long long elapsed_time = end_time - start_time;

    printf("--> solve return: \n\n");
    pprint_sudoku(sudoku);
    printf("Elapsed time: %lld ms\n", elapsed_time);

    return 0;
}
