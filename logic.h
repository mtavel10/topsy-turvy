#ifndef LOGIC_H
#define LOGIC_H

#include <stdbool.h>
#include "board.h"

/* Represents the player whose turn it is or the game's outcome */
enum turn {
    BLACKS_TURN,
    WHITES_TURN
};

typedef enum turn turn;


enum outcome {
    IN_PROGRESS,
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;


struct game {
    unsigned int run;
    board* b;
    posqueue *black_queue, *white_queue;
    turn player;
};

typedef struct game game;


/* Creates a new game with the specified size and configuration, 
 * using the desired data representation.
 * Run parameter configures the length of the run needed to win. 
 * Raises error if run is impossible to achieve. */
game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type);

/* Frees the game. */
void game_free(game* g);

/* Drops a piece belonging to the player whose turn it is in the 
 * specified column. 
 * Returns false if column is full, true otherwise. 
 * Switches the turn to the next player. 
 * Updates the queue to reflect the new piece being placed. */
bool drop_piece(game* g, unsigned int column);

/* Reflects the board across the horizontal centerline, then implements the 
 * effects of gravity in each column. 
 * Updates the queue entries to reflect the new positions. 
 * Switches the turn to the next player */
void disarray(game* g);

/* Removes the oldest piece of the current player and the 
 * newest piece of their opponent. 
 * Updates the locations above removed pieces according to gravity. 
 * Updates the queue entries to reflect the new positions. 
 * Changes the oldest piece of the player who performed the move to the 
 * first one placed after the removed piece. Opponent's second-newest piece 
 * becomes the newest. 
 * Returns false if one or both players have no pieces on the board, 
 * true otherwise. */
bool offset(game* g);

/* Reports the outcome of a completed game or if its still in progress */
outcome game_outcome(game* g);

void old_disarray(game* g);

#endif /* LOGIC_H */
