Topsy Turvy is a variant of Connect-4, where players drop pieces into a board and to connect a "run" number of pieces in a row.
Players have the option of flipping the board horizontally ("Disarray"), or removing their opponents newest move and their own oldest move ("Offset") from the board. 
Players play the game through the terminal (TUI) and drop pieces by typing the number (or letter) column they wish to drop their piece into. 

Users must specify the board's width, height, winning run size, and board storage type in the command line arguments. A sample of the proper command line input is included below:

./play -w 10 -h 10 -r 4 -m
In this sample, the program creates a 10x10 board, with a winning run size of 4, and the board's internal storage is in a 2D matrix.
The tag -b can be used instead of -m, which stores the board in bits rather than a 2D matrix. Both internal storage systems appear the same to the users. 

This was my final project for CMSC14300: Systems Programming I. 
It's coded entirely in C and features key systems programming concepts including linked lists, queues, matrices, recursion, bitwise operations and representations, bitmasking, command line args, a text-user interface, C structs, and a robust test suite. 
