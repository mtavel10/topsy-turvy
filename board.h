#ifndef BOARD_H
#define BOARD_H

#include "pos.h"

/* Represents the contents of a given location on the board */
enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;


union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};


struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* Helper function for bit representation testing. */
char* dec_to_bin(unsigned int n);

/* Creates a fully-empty board of the desired size. 
 * For now, raises an error if the BITS representation is chosen */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* Fully frees the board and its internal representation.
 * For now, raises an error if BITS representation is used. */
void board_free(board* b);

/* Prints the board to the screen with row and column headers.
 * Blank horizonal and vertical lines separate headers from board. 
 * Headers are represented as digits 0-9, A-Z, a-z, then ?s 
 * For now, raises an error if BITS representation is used */
void board_show(board* b);

/* Retrieves a cell within the board. 
 * Raises error if out of bounds 
 * For now, raises an error if BITS reprsentation is used */
cell board_get(board* b, pos p);

/* Modifies a cell within the board. 
 * Raises error if out of bounds 
 * For now, raises an eror if BITS representation is used */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
