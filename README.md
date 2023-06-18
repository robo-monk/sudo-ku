# SUDO-KU


This project is a Sudoku solver written in C using bitboards to efficiently represent the Sudoku grid and its possible values. The program solves any valid Sudoku puzzle by recursively testing all possible values for each empty square.



# Usage

To run the program, clone the repository and then run the following command:
```
make && ./bin/sudo-ku
```

The program can solve any valid Sudoku puzzle. To use it, simply provide the puzzle as a string of digits, where empty squares are represented by the char `0` or `.` or any other non \[1-9\] char. For example:

```c
char puzzle[] = "000000002001000700030050090000006040003040800040509000090060030002000100700003000";
Sudoku sudoku = newSudoku();
load_sudoku(puzzle, &sudoku);
solve(&sudoku);
pprint_sudoku(sudoku);
```


This will solve the following Sudoku puzzle:
```
╭--⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯⎯-╮
⏐ _  _  _ ⏐ _  _  _ ⏐ _  _  2 |
⏐ _  _  1 ⏐ _  _  _ ⏐ 7  _  _ |
⏐ _  3  _ ⏐ _  5  _ ⏐ _  9  _ |
+-⎯-⎯--⎯+-⎯⎯⎯⎯⎯+⎯⎯⎯⎯-⎯+
⏐ _  _  _ ⏐ _  _  6 ⏐ _  4  _ |
⏐ _  _  3 ⏐ _  4  _ ⏐ 8  _  _ |
⏐ _  4  _ ⏐ 5  _  9 ⏐ _  _  _ |
+-⎯-⎯--⎯+-⎯⎯⎯⎯⎯+⎯⎯⎯⎯-⎯+
⏐ _  9  _ ⏐ _  6  _ ⏐ _  3  _ |
⏐ _  _  2 ⏐ _  _  _ ⏐ 1  _  _ |
⏐ 7  _  _ ⏐ _  _  3 ⏐ _  _  _ |
╰-⎯⎯⎯⎯⎯⎯⎯⎯⎯-⎯⎯⎯⎯⎯⎯⎯-╯
```



# Implementation

The program uses bitboards to represent the Sudoku grid and the possible values for each square. Bitboards are a compact and efficient way to store and manipulate sets of bits, which makes them ideal for this type of problem.

The program uses a recursive backtracking algorithm to solve the puzzle. At each step, it selects an empty square and tries all possible values for that square. If a solution is found, the program terminates and returns the completed Sudoku grid. If no solution is found, the program backtracks and tries a different value for the previous square.
