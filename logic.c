#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "pos.h"
#include "board.h"
#include "logic.h"

/* Creates a new game with the specified size and configuration, 
 * using the desired data representation. Run parameter configures the length 
 * of the run needed to win. Raises error if run is impossible to achieve. */
game* new_game(unsigned int run, unsigned int width,
               unsigned int height, enum type type){
                if (run > width && run > height){
                    fprintf(stderr, 
                                "new_game: run is impossible on this board\n");
                    exit(1);
                }
                game *g = (game*)malloc(sizeof(game));
                if (g == NULL){
                    fprintf(stderr, "new_game: malloc error");
                    exit(1);
                }
                g -> run = run;
                g -> b = board_new(width, height, type);
                g -> black_queue = posqueue_new();
                g -> white_queue = posqueue_new();
                // black goes first
                g -> player = BLACKS_TURN;
                return g;
}

/* Frees the game. */
void game_free(game* g){
    board_free(g -> b);
    posqueue_free(g -> black_queue);
    posqueue_free(g -> white_queue);
    free(g);
}

/* Helper function to switch the turns of the players */
void switch_player_turn(game* g){
    if (g -> player == WHITES_TURN){
        g -> player = BLACKS_TURN;
    }
    else {
        g -> player = WHITES_TURN;
    }
}

/* Drops a piece in the specified columnn of the current player. 
 * Returns false if column is full, true otherwise. 
 * Switches the turn to the next player. 
 * Updates the queue to reflect the new piece being placed. */
bool drop_piece(game* g, unsigned int column){
    int row;
    // Starts indexing from the bottom and moves up
    for (row = (g -> b -> height) - 1; row >= 0; row--){
        // Drops the piece in the lowest available spot in the column
        pos p = make_pos(row, column);
        if (board_get(g -> b, p) == EMPTY) {
            if (g -> player == WHITES_TURN) {
                board_set(g -> b, p, WHITE);
                pos_enqueue(g -> white_queue, p);
            }
            else {
                board_set(g -> b, p, BLACK);
                pos_enqueue(g -> black_queue, p);
            }
            
            switch_player_turn(g);

            return true;
        }
    }
    return false;
}

struct args{
    game* g;
    unsigned int c;
};

void* disarray_wrapper(void* a){
    struct args* args = (struct args*)a;
    game* g = args -> g;
    unsigned int col = (args -> c);
    // Flips each column by removing the pieces / dropping them back in reverse
    // Removes the positions from the given column from the top to the bottom
    pos removed_positions[g -> b -> height];
    cell removed_cells[g -> b -> height];
    unsigned int removed_count = 0;
    for (int row = 0; row < (g -> b -> height); row++){
        pos p = make_pos(row, col);
        cell c = board_get(g -> b, p);
        if (c != EMPTY){
            removed_positions[removed_count] = p;
            removed_cells[removed_count] = c;
            removed_count++;
            board_set(g -> b, p, EMPTY);
        }
    }

    // Drops the positions in the order in which they were removed
    // Creates a list of new positions that the old cells are now at
    pos new_positions[removed_count];
    unsigned int i = 0;
    int start_row = (g -> b -> height) - 1;
    int end_row = (g -> b -> height) - removed_count;
    for (int row = start_row; row >= end_row && row >= 0; row--){
        pos new_p = make_pos(row, col);
        board_set(g -> b, new_p, removed_cells[i]);
        new_positions[i] = new_p;
        i++;
    }

    // Updates the white queue
    unsigned int j;
    pq_entry *curr_entry = g -> white_queue -> head;
    for (j = 0; j < (g -> white_queue -> len); j++){
        pos curr_p = curr_entry -> p;
        // Search the removed list to see if this position was removed
        unsigned int k;
        for (k = 0; k < removed_count; k++){
            if (pos_cmp(removed_positions[k], curr_p)){
                // Update the current entry's position to its new one
                curr_entry -> p = new_positions[k];
            }
        }
        // Move the current entry to the next one
        curr_entry = curr_entry -> next;
    }

    // Updates the black queue
    unsigned int l;
    curr_entry = g -> black_queue -> head;
    for (l = 0; l < (g -> black_queue -> len); l++){
        pos curr_p = curr_entry -> p;
        // Search the removed list to see if this position was removed
        unsigned int m;
        for (m = 0; m < removed_count; m++){
            if (pos_cmp(removed_positions[m], curr_p)){
                // Update the current entry's position to its new one
                curr_entry -> p = new_positions[m];
            }
        }
        // Move the current entry to the next one
        curr_entry = curr_entry -> next;
    }

    return NULL;
}

/* Reflects the board across the horizontal centerline,  
 * then implements the effects of gravity in each column. 
 * Updates the queue entries to reflect the new positions. 
 * Switches the turn to the next player */
void disarray(game* g){
    if (g -> b -> type == MATRIX){
        unsigned int cols = g -> b -> width;
        pthread_t threads[cols];
        struct args args[cols];
        
        unsigned int i;
        for (i = 0; i < cols; i++){
            args[i].g = g;
            args[i].c = i;
            pthread_create(&threads[i], NULL, disarray_wrapper, &args[i]);
        }
        for (i = 0; i < cols; i++){
            void* thdres;
            pthread_join(threads[i], &thdres);
            free(thdres);
        }
    }
    else{ // No threading on a bits board
        // Flip each column by removing the positions and dropping them back in reverse
        unsigned int col;
        for (col = 0; col < (g -> b -> width); col++){
            // Removes the positions from the given column from the top to the bottom
            pos removed_positions[g -> b -> height];
            cell removed_cells[g -> b -> height];
            unsigned int removed_count = 0;
            for (int row = 0; row < (g -> b -> height); row++){
                pos p = make_pos(row, col);
                cell c = board_get(g -> b, p);
                if (c != EMPTY){
                    removed_positions[removed_count] = p;
                    removed_cells[removed_count] = c;
                    removed_count++;
                    board_set(g -> b, p, EMPTY);
                    
                }
            }

            // Drops the positions in the order in which they were removed
            // Creates a list of new positions that the old cells are now at
            pos new_positions[removed_count];
            unsigned int i = 0;
            int start_row = (g -> b -> height) - 1;
            int end_row = (g -> b -> height) - removed_count;
            for (int row = start_row; row >= end_row && row >= 0; row--){
                pos new_p = make_pos(row, col);
                board_set(g -> b, new_p, removed_cells[i]);
                new_positions[i] = new_p;
                i++;
            }

            // Updates the white queue
            unsigned int j;
            pq_entry *curr_entry = g -> white_queue -> head;
            for (j = 0; j < (g -> white_queue -> len); j++){
                pos curr_p = curr_entry -> p;
                // Search the removed list to see if this position was removed
                unsigned int k;
                for (k = 0; k < removed_count; k++){
                    if (pos_cmp(removed_positions[k], curr_p)){
                        // Update the current entry's position to its new one
                        curr_entry -> p = new_positions[k];
                    }
                }
                // Move the current entry to the next one
                curr_entry = curr_entry -> next;
            }

            // Updates the black queue
            unsigned int l;
            curr_entry = g -> black_queue -> head;
            for (l = 0; l < (g -> black_queue -> len); l++){
                pos curr_p = curr_entry -> p;
                // Search the removed list to see if this position was removed
                unsigned int m;
                for (m = 0; m < removed_count; m++){
                    if (pos_cmp(removed_positions[m], curr_p)){
                        // Update the current entry's position to its new one
                        curr_entry -> p = new_positions[m];
                    }
                }
                // Move the current entry to the next one
                curr_entry = curr_entry -> next;
            }
        }
    }
    switch_player_turn(g);
}

/* Offset helper function that shifts down a column after a piece is removed */
void shift_down(game* g, pos p){
    if (p.r == 0){
        board_set(g -> b, p, EMPTY);
        return;
    }
    pos p_above = make_pos(p.r - 1, p.c);
    if (board_get(g -> b, p_above) == EMPTY){
        board_set(g -> b, p, EMPTY);
        return;
    }
    else {
        // Sets the current equal to the one above (shifts down one)
        board_set(g -> b, p, board_get(g -> b, p_above));
        shift_down(g, p_above);
    }
}

/* Removes the current player's oldest and opponent's newest. 
 * Updates the locations above removed pieces according to gravity. 
 * Updates the queue entries to reflect the new positions. 
 * Changes the oldest piece of the player who performed the move to the 
 * first one placed after the removed piece. 
 * Opponent's second-newest piece becomes the newest. 
 * Returns false if one or both players have no pieces on the board, 
 * true otherwise. */
bool offset(game* g){
    // False if one or both players have no pieces on the board, true otherwise
    if (g -> black_queue -> len == 0 || g -> white_queue -> len == 0){
        return false;
    }
    
    // Finds the pieces to be removed, updates the queues
    pos curr_oldest;
    pos opp_newest;
    if (g -> player == WHITES_TURN) {
        curr_oldest = pos_dequeue(g -> white_queue);
        //printf("current oldest: %d, %d\n", curr_oldest.r, curr_oldest.c);
        opp_newest = posqueue_remback(g -> black_queue);
        //printf("opponent newest: %d, %d\n", opp_newest.r, opp_newest.c);
    }
    else {
        curr_oldest = pos_dequeue(g -> black_queue);
        opp_newest = posqueue_remback(g -> white_queue);
    }
    // Removes the pieces at these positions
    board_set(g -> b, curr_oldest, EMPTY);
    board_set(g -> b, opp_newest, EMPTY);
    shift_down(g, curr_oldest);
    shift_down(g, opp_newest);
  
    // updates the white queue
    pq_entry* entry = g -> white_queue -> head;
    for (int i = 0; i < g -> white_queue -> len; i++){
        // only shift down the positions in a column
        // beneath where the piece was removed
        if (entry -> p.c == curr_oldest.c && entry -> p.r < curr_oldest.r){
            pos curr_pos = entry -> p;
            entry -> p = make_pos(curr_pos.r + 1, curr_pos.c);
        }
        if (entry -> p.c == opp_newest.c && entry -> p.r < opp_newest.r){
            pos curr_pos = entry -> p;
            entry -> p = make_pos(curr_pos.r + 1, curr_pos.c);
        }
        entry = entry -> next;
    }
    

    // updates the black queue
    entry = g -> black_queue -> head;
    for (int i = 0; i < g -> black_queue -> len; i++){
        // only shift down the positions in a column
        // beneath where the piece was removed
        if (entry -> p.c == curr_oldest.c && entry -> p.r < curr_oldest.r){
            pos curr_pos = entry -> p;
            entry -> p = make_pos(curr_pos.r + 1, curr_pos.c);
        }
        if (entry -> p.c == opp_newest.c && entry -> p.r < opp_newest.r){
            pos curr_pos = entry -> p;
            entry -> p = make_pos(curr_pos.r + 1, curr_pos.c);
        }
        entry = entry -> next;
    }

    switch_player_turn(g);
    return true; 
}

/* Helper functions for game_outcome. 
 * Check whether there is a winning run in four directions: right, down, 
 * right/down diagonal, and left/down diagonal*/
bool check_right(game* g, pos p, int r){
    // base cases
    // this is a winning run
    if (r == g -> run){
        return true;
    }
    // this piece is on the edge (and not part of a winning run)
    else if (p.c + 1 == g -> b -> width){
        return false;
    }
    // this piece doesn't match the next
    else if (board_get(g -> b, p) != board_get(g -> b, make_pos(p.r, p.c + 1))){
        return false;
    }
    // this piece matches the next, so let's add 1 to the run and check the next
    else{
        return check_right(g, make_pos(p.r, p.c + 1), r + 1);
    }
}

bool check_down_right(game* g, pos p, int r){
    if (r == g -> run){
        return true;
    }
    else if (p.c + 1 == g -> b -> width || p.r + 1 == g -> b -> height){
        return false;
    }
    else if (board_get(g -> b, p) != board_get(g -> b, 
                                               make_pos(p.r + 1, p.c + 1))){
        return false;
    }
    else{
        return check_down_right(g, make_pos(p.r + 1, p.c + 1), r + 1);
    }
}

bool check_down(game* g, pos p, int r){
    if (r == g -> run){
        return true;
    }
    else if (p.r + 1 == g -> b -> height){
        return false;
    }
    else if (board_get(g -> b, p) != board_get(g -> b, make_pos(p.r + 1, p.c))){
        return false;
    }
    else{
        return check_down(g, make_pos(p.r + 1, p.c), r + 1);
    }
}

bool check_down_left(game* g, pos p, int r){
    if (r == g -> run){
        return true;
    }
    else if (p.c == 0 || p.r + 1 == g -> b -> height){
        return false;
    }
    else if (board_get(g -> b, p) != board_get(g -> b, 
                                               make_pos(p.r + 1, p.c - 1))){
        return false;
    }
    else {
        return check_down_left(g, make_pos(p.r + 1, p.c - 1), r + 1);
    }
}

/* Reports the outcome of a completed game or if its still in progress */
outcome game_outcome(game* g){
    // Iterates through the board to check each cell for a winning run
    bool black_won = false;
    bool white_won = false;
    unsigned int row, col; 
    for (row = 0; row < g -> b -> height; row++){
        for (col = 0; col < g -> b -> width; col++){

            pos curr_pos = make_pos(row, col);
            cell curr_cell = board_get(g -> b, curr_pos);
        
            if (curr_cell != EMPTY){
                bool winning_run_found = check_right(g, curr_pos, 1) ||
                                         check_down_right(g, curr_pos, 1) || 
                                         check_down(g, curr_pos, 1) || 
                                         check_down_left(g, curr_pos, 1);
                if (winning_run_found){
                    if (curr_cell == WHITE){
                        white_won = true;
                    }
                    else if (curr_cell == (BLACK)){
                        black_won = true;
                    }
                }
            }
        }
    }

    unsigned int board_spaces = (g -> b -> width) * (g -> b -> height);
    unsigned int filled_spaces = (g -> black_queue -> len) + 
                                 (g -> white_queue -> len);

    // both won
    if (white_won && black_won){
        return DRAW;
    }
    // board is filled and neither won
    else if (!white_won && !black_won && board_spaces == filled_spaces){
        return DRAW;
    }
    // black won
    else if (black_won){
        return BLACK_WIN;
    }
    // white won
    else if (white_won){
        return WHITE_WIN;
    }
    return IN_PROGRESS;
}

char* outcome_to_str(outcome o){
    if (o == BLACK_WIN){
        return "outcome: black won\n";
    }
    else if (o == WHITE_WIN){
        return "outcome: white won\n";
    }
    else if (o == DRAW){
        return "outcome: draw\n";
    }
    else {
        return "outcome: in progress\n";
    }
}

void old_disarray(game* g){
    // Flip each column by removing the positions and dropping them back in reverse
    unsigned int col;
    for (col = 0; col < (g -> b -> width); col++){
        // Removes the positions from the given column from the top to the bottom
        pos removed_positions[g -> b -> height];
        cell removed_cells[g -> b -> height];
        unsigned int removed_count = 0;
        for (int row = 0; row < (g -> b -> height); row++){
            pos p = make_pos(row, col);
            cell c = board_get(g -> b, p);
            if (c != EMPTY){
                removed_positions[removed_count] = p;
                removed_cells[removed_count] = c;
                removed_count++;
                board_set(g -> b, p, EMPTY);
            }
        }

        // Drops the positions in the order in which they were removed
        // Creates a list of new positions that the old cells are now at
        pos new_positions[removed_count];;
        unsigned int i = 0;
        int start_row = (g -> b -> height) - 1;
        int end_row = (g -> b -> height) - removed_count;
        for (int row = start_row; row >= end_row && row >= 0; row--){
            pos new_p = make_pos(row, col);
            board_set(g -> b, new_p, removed_cells[i]);
            new_positions[i] = new_p;
            i++;
        }

        // Updates the white queue
        unsigned int j;
        pq_entry *curr_entry = g -> white_queue -> head;
        for (j = 0; j < (g -> white_queue -> len); j++){
            pos curr_p = curr_entry -> p;
            // Search the removed list to see if this position was removed
            unsigned int k;
            for (k = 0; k < removed_count; k++){
                if (pos_cmp(removed_positions[k], curr_p)){
                    // Update the current entry's position to its new one
                    curr_entry -> p = new_positions[k];
                }
            }
            // Move the current entry to the next one
            curr_entry = curr_entry -> next;
        }

        // Updates the black queue
        unsigned int l;
        curr_entry = g -> black_queue -> head;
        for (l = 0; l < (g -> black_queue -> len); l++){
            pos curr_p = curr_entry -> p;
            // Search the removed list to see if this position was removed
            unsigned int m;
            for (m = 0; m < removed_count; m++){
                if (pos_cmp(removed_positions[m], curr_p)){
                    // Update the current entry's position to its new one
                    curr_entry -> p = new_positions[m];
                }
            }
            // Move the current entry to the next one
            curr_entry = curr_entry -> next;
        }
    }

    switch_player_turn(g);
}
