#include <criterion/criterion.h>
#include <stdio.h>
#include "pos.h"
#include "board.h"
#include "logic.h"
#include <sys/time.h>

/* Pos Tests: make_pos, posqueue_new, pos_enqueue, pos_dequeue, posqueue_remback, posqueue_free */

posqueue* make_posqueue1(){
	posqueue* q = posqueue_new();
	pos_enqueue(q, make_pos(0, 0));
	return q;
}

posqueue* make_posqueue2(){
	posqueue* q = posqueue_new();
	pos_enqueue(q, make_pos(0, 0));
	pos_enqueue(q, make_pos(0, 1));
	return q;
}

posqueue* make_posqueue4(){
	posqueue* q = posqueue_new();
	pos_enqueue(q, make_pos(0, 0));
	pos_enqueue(q, make_pos(0, 1));
	pos_enqueue(q, make_pos(1, 0));
	pos_enqueue(q, make_pos(1, 1));
	return q;
}

Test(make_pos, test1) {
	pos test_pos;
	test_pos.r = 1;
	test_pos.c = 2;
	cr_assert(pos_cmp(test_pos, make_pos(1, 2)));
}

Test(make_pos, test2) {
	pos test_pos;
	test_pos.r = 5;
	test_pos.c = 3;
	cr_assert(!pos_cmp(test_pos, make_pos(3, 5)));
}

Test(posqueue_new, test1) {
	posqueue* q = posqueue_new();
	cr_assert(q -> head == NULL);
	cr_assert(q -> tail == NULL);
	cr_assert(q -> len == 0);
}

Test(pos_enqueue, test1) {
	posqueue* q = make_posqueue1();
	cr_assert(pos_cmp(q -> head -> p, make_pos(0, 0)));
	cr_assert(pos_cmp(q -> tail -> p, make_pos(0, 0)));
	cr_assert(q -> head -> prev == NULL);
	cr_assert(q -> tail -> next == NULL);
	cr_assert(q -> len == 1);
	posqueue_free(q); 
}

Test(pos_enqueue, test2) {
	posqueue* q = make_posqueue2();
	cr_assert(pos_cmp(q -> head -> p, make_pos(0, 0)));
	cr_assert(pos_cmp(q -> head -> next -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(q -> tail -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(q -> tail -> prev -> p, make_pos(0, 0)));
	cr_assert(q -> head -> prev == NULL);
	cr_assert(q -> tail -> next == NULL);
	cr_assert(q -> len == 2);
	posqueue_free(q); 
}

Test(pos_enqueue, test3) {
	posqueue* q = make_posqueue4();
	cr_assert(pos_cmp(q -> head -> p, make_pos(0, 0)));
	cr_assert(pos_cmp(q -> head -> next -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(q -> head -> next -> next -> p, make_pos(1, 0)));
	cr_assert(pos_cmp(q -> head -> next -> next -> next -> p, make_pos(1, 1)));
	cr_assert(pos_cmp(q -> tail -> p, make_pos(1, 1)));
	cr_assert(pos_cmp(q -> tail -> prev -> p, make_pos(1, 0)));
	cr_assert(pos_cmp(q -> tail -> prev -> prev -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(q -> tail -> prev -> prev -> prev -> p, make_pos(0, 0)));
	cr_assert(q -> len == 4);
	posqueue_free(q); 
}

Test(pos_dequeue, test1) {
	posqueue* q = make_posqueue1();
	cr_assert(pos_cmp(pos_dequeue(q), make_pos(0, 0)));
	cr_assert(q -> head == NULL);
	cr_assert(q -> tail == NULL);
	cr_assert(q -> len == 0);
	posqueue_free(q); 
}

Test(pos_dequeue, test2) {
	posqueue* q = make_posqueue2();
	cr_assert(pos_cmp(pos_dequeue(q), make_pos(0, 0)));
	cr_assert(pos_cmp(q -> head -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(q -> tail -> p, make_pos(0, 1)));
	cr_assert(q -> head -> next == NULL);
	cr_assert(q -> head -> prev == NULL);
	cr_assert(q -> tail -> next == NULL);
	cr_assert(q -> tail -> prev == NULL);
	cr_assert(q -> len == 1);
	posqueue_free(q); 
}

Test(pos_dequeue, test3) {
	posqueue* q = make_posqueue4();
	cr_assert(pos_cmp(pos_dequeue(q), make_pos(0, 0)));
	cr_assert(pos_cmp(q -> head -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(q -> head -> next -> p, make_pos(1, 0)));
	cr_assert(q -> head -> prev == NULL);
	cr_assert(q -> len == 3);
	posqueue_free(q); 
}

Test(posqueue_remback, test1) {
	posqueue* q = make_posqueue1();
	cr_assert(pos_cmp(posqueue_remback(q), make_pos(0, 0)));
	cr_assert(q -> head == NULL);
	cr_assert(q -> tail == NULL);
	cr_assert(q -> len == 0);
	posqueue_free(q); 
}

Test(posqueue_remback, test2) {
	posqueue* q = make_posqueue2();
	cr_assert(pos_cmp(posqueue_remback(q), make_pos(0, 1)));
	cr_assert(pos_cmp(q -> tail -> p, make_pos(0, 0)));
	cr_assert(q -> tail -> next == NULL);
	cr_assert(q -> tail -> prev == NULL);
	cr_assert(q -> len == 1);
	posqueue_free(q); 
}

Test(posqueue_remback, test3) {
	posqueue* q = make_posqueue4();
	cr_assert(pos_cmp(posqueue_remback(q), make_pos(1, 1)));
	cr_assert(pos_cmp(q -> tail -> p, make_pos(1, 0)));
	cr_assert(q -> tail -> next == NULL);
	cr_assert(pos_cmp(q -> tail -> prev -> p, make_pos(0, 1)));
	cr_assert(q -> len == 3);
	posqueue_free(q); 
}

Test(posqueue_free, test1) {
	posqueue* q = make_posqueue1();
	posqueue_free(q);
	cr_assert(1);
}

Test(posqueue_free, test2) {
	posqueue* q = make_posqueue2();
	posqueue_free(q);
	cr_assert(1);
}

Test(posqueue_free, test3) {
	posqueue* q = make_posqueue4();
	posqueue_free(q);
	cr_assert(1);
}

/* Board Tests: board_new, board_free, board_show, board_get, board_set */

Test(board_new, test1){
	board* b = board_new(1, 1, MATRIX);
	cr_assert(b -> width == 1);
	cr_assert(b -> height == 1);
	cr_assert(b -> type == MATRIX);
	cell** matrix = b -> u.matrix;
	unsigned int i;
	for (i = 0; i < 1; i++){
		unsigned int j;
		for (j = 0; j < 1; j++){
			cr_assert(matrix[i][j] == EMPTY);
		}
	}
}

Test(board_new, test1_BITS){
	board* b = board_new(1, 1, BITS);
	cr_assert(b -> width == 1);
	cr_assert(b -> height == 1);
	cr_assert(b -> type == BITS);
	unsigned int i;
	for (i = 0; i < (b -> width * b -> height); i++){
		cr_assert((b -> u.bits[i]) == 0);
	}
}

Test(board_new, test2){
	board* b = board_new(5, 5, MATRIX);
	cr_assert(b -> width == 5);
	cr_assert(b -> height == 5);
	cr_assert(b -> type == MATRIX);
	unsigned int i, j;
	cell** matrix = b -> u.matrix;
	for (i = 0; i < 5; i++){
		for (j = 0; j < 5; j++){
			cr_assert(matrix[i][j] == EMPTY);
		}
	}
	board_free(b);
}

Test(board_new, test3){
	board* b = board_new(3, 4, MATRIX);
	cr_assert(b -> width == 3);
	cr_assert(b -> height == 4);
	cr_assert(b -> type == MATRIX);
	cell** matrix = b -> u.matrix;
	unsigned int i, j;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 3; j++){
			cr_assert(matrix[i][j] == EMPTY);
		}
	}
	board_free(b);
}

Test(board_free, test1){
	board* b = board_new(5, 5, MATRIX);
	board_free(b);
	cr_assert(1);
}

Test(board_free, test2){
	board* b = board_new(5, 5, BITS);
	board_free(b);
	cr_assert(1);
}

Test(board_show, test1){
	board* b = board_new(9, 9, MATRIX);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test2){
	board* b = board_new(11, 11, MATRIX);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test3){
	board* b = board_new(39, 39, MATRIX);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test4){
	board* b = board_new(100, 100, MATRIX);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test5){
	board* b = board_new(5, 5, MATRIX);
	b -> u.matrix[0][0] = BLACK;
	b -> u.matrix[1][1] = WHITE;
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test6){
	board* b = board_new(9, 9, BITS);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test7){
	board* b = board_new(11, 11, BITS);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test8){
	board* b = board_new(39, 39, BITS);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test9){
	board* b = board_new(100, 100, BITS);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_show, test10){
	board* b = board_new(5, 5, BITS);
	board_set(b, make_pos(1, 1), WHITE);
	board_set(b, make_pos(2, 2), BLACK);
	board_show(b);
	cr_assert(1);
	board_free(b);
}

Test(board_get, test1){
	board* b = board_new(5, 5, MATRIX);
	cell c = board_get(b, make_pos(4, 4));
	cr_assert(c == EMPTY);
	board_free(b);
}

Test(board_get, test2){
	board* b = board_new(5, 5, MATRIX);
	b -> u.matrix[2][3] = WHITE;
	cell c = board_get(b, make_pos(2, 3));
	cr_assert(c == WHITE);
	board_free(b);
}

Test(board_get, test3){
	board* b = board_new(5, 5, MATRIX);
	b -> u.matrix[4][4] = BLACK;
	cell c = board_get(b, make_pos(4, 4));
	cr_assert(c == BLACK);
	board_free(b);
}

Test(board_get, BITStest1){
	board* b = board_new(5, 5, BITS);
	cell c = board_get(b, make_pos(4, 4));
	cr_assert(c == EMPTY);
	board_free(b);
}

Test(board_get, BITStest2){
	board* b = board_new(4, 4, BITS);
	b -> u.bits[0] = 1;
	cell c = board_get(b, make_pos(0, 0));
	cr_assert(c == BLACK);
	board_free(b);
}

Test(board_get, BITStest3){
	board* b = board_new(4, 4, BITS);
	b -> u.bits[0] = 2;
	cell c = board_get(b, make_pos(0, 0));
	cr_assert(c == WHITE);
	board_free(b);
}

Test(board_get, BITStest4){
	board* b = board_new(4, 4, BITS);
	b -> u.bits[0] = (1 << 14);
	cell c = board_get(b, make_pos(1, 3));
	cr_assert(c == BLACK);
	board_free(b);
}

Test(board_get, BITStest5){
	board* b = board_new(4, 4, BITS);
	b -> u.bits[0] = (1 << 15);
	cell c = board_get(b, make_pos(1, 3));
	cr_assert(c == WHITE);
	board_free(b);
}

Test(board_get, BITStest6){
	board* b = board_new(4, 4, BITS);
	b -> u.bits[0] = (1 << 4);
	cell c = board_get(b, make_pos(0, 2));
	cr_assert(c == BLACK);
	board_free(b);
}

Test(board_get, BITStest7){
	board* b = board_new(4, 4, BITS);
	b -> u.bits[0] = (1 << 21);
	cell c = board_get(b, make_pos(2, 2));
	cr_assert(c == WHITE);
	board_free(b);
}

Test(board_get, BITStest8){
	board* b = board_new(5, 5, BITS);
	b -> u.bits[1] = 1;
	cell c = board_get(b, make_pos(3, 1));
	cr_assert(c == BLACK);
	board_free(b);
}

Test(board_get, BITStest9){
	board* b = board_new(9, 9, BITS);
	b -> u.bits[5] = 1;
	cell c = board_get(b, make_pos(8, 8));
	cr_assert(c == BLACK);
	board_free(b);
}

Test(board_get, BITStest10){
	board* b = board_new(100, 100, BITS);
	cell c = board_get(b, make_pos(69, 23));
	cr_assert(c == EMPTY);
	board_free(b);
}

Test(board_set, test1){
	board* b = board_new(5, 5, MATRIX);
	board_set(b, make_pos(2, 1), BLACK);
	cr_assert(b -> u.matrix[2][1] == BLACK);
	board_free(b);
}

Test(board_set, test2){
	board* b = board_new(5, 5, MATRIX);
	board_set(b, make_pos(3, 0), WHITE);
	cr_assert(b -> u.matrix[3][0] == WHITE);
	board_free(b);
}

Test(board_set, BITStest1){
	board* b = board_new(4, 4, BITS);
	board_set(b, make_pos(0, 0), BLACK);
	cr_assert(b -> u.bits[0] == 1);
	board_free(b);
}

Test(board_set, BITStest2){
	board* b = board_new(4, 4, BITS);
	board_set(b, make_pos(0, 0), WHITE);
	cr_assert(b -> u.bits[0] == 2);
	board_free(b);
}

Test(board_set, BITStest3){
	board* b = board_new(4, 4, BITS);
	board_set(b, make_pos(1, 1), BLACK);
	cr_assert(((b -> u.bits[0] >> 10) & 0x3) == 1);
	board_free(b);
}

Test(board_set, BITStest4){
	board* b = board_new(4, 4, BITS);
	board_set(b, make_pos(1, 1), WHITE);
	cr_assert(((b -> u.bits[0] >> 10) & 0x3) == 2);
	board_free(b);
}

Test(board_set, BITStest7){
	board* b = board_new(4, 4, BITS);
	board_set(b, make_pos(1, 1), WHITE);
	board_set(b, make_pos(1, 1), EMPTY);
	cr_assert(((b -> u.bits[0] >> 10) & 0x3) == 0);
	board_free(b);
}

Test(board_set, BITStest5){
	board* b = board_new(5, 5, BITS);
	board_set(b, make_pos(4, 2), BLACK);
	cr_assert(((b -> u.bits[1] >> 12) & 0x3) == 1);
	board_free(b);
}

Test(board_set, BITStest6){
	board* b = board_new(5, 5, BITS);
	board_set(b, make_pos(3, 4), WHITE);
	cr_assert(((b -> u.bits[1] >> 6) & 0x3) == 2);
	board_free(b);
}

/*Logic tests: new_game, game_free, drop_piece, disarray, offset, 
 * game_outcome */

Test(new_game, test1){
	game* g = new_game(5, 5, 5, MATRIX);
	cr_assert(g -> run == 5);
	cr_assert(g -> b -> width == 5);
	cr_assert(g -> b -> height == 5);
	cr_assert(g -> b -> type == MATRIX);
	cr_assert(g -> black_queue -> head == NULL);
	cr_assert(g -> black_queue -> tail == NULL);
	cr_assert(g -> black_queue -> len == 0);
	cr_assert(g -> white_queue -> head == NULL);
	cr_assert(g -> white_queue -> tail == NULL);
	cr_assert(g -> white_queue -> len == 0);
	cr_assert(g -> player == BLACKS_TURN);
	game_free(g);
}

Test(game_free, test1){
	game* g = new_game(5, 5, 5, MATRIX);
	game_free(g);
	cr_assert(1);
}

Test(game_free, test2){
	game* g = new_game(5, 5, 5, BITS);
	game_free(g);
	cr_assert(1);
}

Test(drop_piece, test1){
	game* g = new_game(4, 4, 4, MATRIX);
	cr_assert(drop_piece(g, 0));
	cr_assert(g -> player == WHITES_TURN);
	cr_assert(board_get(g -> b, make_pos(3, 0)) == BLACK);
	cr_assert(pos_cmp(g -> black_queue -> head -> p, make_pos(3, 0)));
	game_free(g);
}

Test(drop_piece, test2){
	game* g = new_game(4, 4, 4, MATRIX);
	cr_assert(drop_piece(g, 0));
	cr_assert(drop_piece(g, 0));
	cr_assert(g -> player == BLACKS_TURN);
	cr_assert(board_get(g -> b, make_pos(3, 0)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(2, 0)) == WHITE);
	cr_assert(pos_cmp(g -> black_queue -> head -> p, make_pos(3, 0)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(2, 0)));
	game_free(g);
}

Test(drop_piece, test3){
	game* g = new_game(4, 4, 4, MATRIX);
	cr_assert(drop_piece(g, 0)); // black
	cr_assert(drop_piece(g, 0)); // white
	cr_assert(drop_piece(g, 3)); // black
	cr_assert(g -> player == WHITES_TURN);
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> p, make_pos(3, 3)));
	game_free(g);
}

Test(drop_piece, test4){
	game* g = new_game(2, 2, 2, MATRIX);
	cr_assert(drop_piece(g, 0)); // black
	cr_assert(drop_piece(g, 0)); // white
	cr_assert(!drop_piece(g, 0)); 
	cr_assert(g -> player == BLACKS_TURN);
	game_free(g);
}

Test(disarray, test1){
	game* g = new_game(2, 2, 2, BITS);
	drop_piece(g, 0); // black (1, 0)
	drop_piece(g, 0); // white (0, 0)
	disarray(g); // black flips the positions
	cr_assert(g -> player == WHITES_TURN);
	cr_assert(board_get(g -> b, make_pos(1, 0)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(0, 0)) == BLACK);
	cr_assert(pos_cmp(g -> black_queue -> head -> p, make_pos(0, 0)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(1, 0)));
	game_free(g);
} 

Test(disarray, test2){
	game* g = new_game(2, 2, 2, BITS);
	drop_piece(g, 0); // black (1, 0)
	drop_piece(g, 1); // white (1, 1)
	drop_piece(g, 0); // black (0, 0)
	drop_piece(g, 1); // white (0, 1)
	disarray(g);
	cr_assert(pos_cmp(g -> black_queue -> head -> p, make_pos(0, 0)));
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> p, make_pos(1, 0)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(g -> white_queue -> head -> next -> p, make_pos(1, 1)));
	game_free(g);
}

Test(disarray, test3){
	game *g = new_game(4, 4, 4, BITS);
	drop_piece(g, 1);
	drop_piece(g, 1);
	drop_piece(g, 2);
	drop_piece(g, 1);
	drop_piece(g, 3);
	drop_piece(g, 2);
	disarray(g);
	cr_assert(board_get(g -> b, make_pos(3, 1)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(2, 1)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(1, 1)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(3, 2)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(2, 2)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(3, 3)) == BLACK);
	cr_assert(pos_cmp(g -> black_queue -> head -> p, make_pos(1, 1)));
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> p, make_pos(2, 2)));
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> next -> p, 
					  make_pos(3, 3)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(2, 1)));
	cr_assert(pos_cmp(g -> white_queue -> head -> next -> p, make_pos(3, 1)));
	cr_assert(pos_cmp(g -> white_queue -> head -> next -> next -> p, 
					  make_pos(3, 2)));
	game_free(g);
}

Test(disarray, test4){
	game *g = new_game(4, 8, 3, BITS);
	drop_piece(g, 1);
	drop_piece(g, 1);
	drop_piece(g, 2);
	drop_piece(g, 1);
	drop_piece(g, 4);
	drop_piece(g, 2);
	disarray(g);
	cr_assert(board_get(g -> b, make_pos(2, 1)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(1, 1)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(0, 1)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(2, 2)) == WHITE);
	cr_assert(board_get(g -> b, make_pos(1, 2)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(2, 4)) == BLACK);
	cr_assert(pos_cmp(g -> black_queue -> head -> p, make_pos(0, 1)));
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> p, make_pos(1, 2)));
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> next -> p, 
					  make_pos(2, 4)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(1, 1)));
	cr_assert(pos_cmp(g -> white_queue -> head -> next -> p, make_pos(2, 1)));
	cr_assert(pos_cmp(g -> white_queue -> head -> next -> next -> p, 
					  make_pos(2, 2)));
	game_free(g);
}

Test(disarray, test5){
	game *g = new_game(4, 4, 4, BITS);
	// fills up the entire board
	for (int i = 0; i <  g -> b -> width; i++){
		for (int j = 0; j < g -> b -> height; j++){
			drop_piece(g, i);
		}
	}
	disarray(g);
	for (int row = 0; row < g -> b -> height; row++){
		for (int col = 0; col < g -> b -> width; col++){
			if (row % 2 == 0){
				cr_assert(board_get(g -> b, make_pos(row, col)) == BLACK);
			}
			else{
				cr_assert(board_get(g -> b, make_pos(row, col)) == WHITE);
			}
		}
	}
	game_free(g);
}

Test(offset, test1){
	game* g = new_game(5, 5, 5, MATRIX);
	cr_assert(!offset(g));
}

Test(offset, test2){
	game* g = new_game(5, 5, 5, MATRIX);
	drop_piece(g, 0); // black (4, 0)
	cr_assert(!offset(g));
}

Test(offset, test3){
	game *g = new_game(5, 5, 5, BITS);
	drop_piece(g, 3); // black (4, 3) 
	drop_piece(g, 3); // white (3, 3)
	cr_assert(offset(g));
	cr_assert(board_get(g -> b, make_pos(4, 3)) == EMPTY);
	cr_assert(board_get(g -> b, make_pos(3, 3)) == EMPTY);
	cr_assert(g -> white_queue -> len == 0);
	cr_assert(g -> black_queue -> len == 0);
	game_free(g);
}

Test(offset, test4){
	game* g = new_game(5, 5, 5, MATRIX);
	drop_piece(g, 0); // black (4, 0)
	drop_piece(g, 0); // white (3, 0)
	drop_piece(g, 0); // black (2, 0)
	drop_piece(g, 1); // white (4, 1)
	drop_piece(g, 1); // black (3, 1)
	// white offsets, removes white's oldest and black's newest
	cr_assert(offset(g));
	cr_assert(board_get(g -> b, make_pos(3, 0)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(2, 0)) == EMPTY);
	cr_assert(board_get(g -> b, make_pos(3, 1)) == EMPTY);
	cr_assert(pos_cmp(g -> black_queue -> tail -> p, make_pos(3, 0)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(4, 1)));
}

Test(offset, test5){
	game *g = new_game(3, 3, 3, MATRIX);
	drop_piece(g, 0); // black 2, 0
	drop_piece(g, 0); // white 1, 0
	drop_piece(g, 0); // black 0, 0
	drop_piece(g, 1); // white 2, 1
	drop_piece(g, 1); // black 1, 1
	drop_piece(g, 1); // white 0, 1
	drop_piece(g, 2); // black 2, 2
	drop_piece(g, 2); // white 1, 2
	drop_piece(g, 2); // black 0, 2
	// white offsets
	cr_assert(offset(g));
	cr_assert(board_get(g -> b, make_pos(0, 0)) == EMPTY);
	cr_assert(board_get(g -> b, make_pos(1, 0)) == BLACK);
	cr_assert(board_get(g -> b, make_pos(0, 2)) == EMPTY);
	cr_assert(pos_cmp(g -> black_queue -> head -> next -> p, make_pos(1, 0)));
	cr_assert(pos_cmp(g -> white_queue -> head -> p, make_pos(2, 1)));
	cr_assert(pos_cmp(g -> black_queue -> tail -> p, make_pos(2, 2)));
	game_free(g);
}

Test(game_outcome, in_progress1){
	game *g = new_game(3, 5, 5, MATRIX);
	drop_piece(g, 0);
	drop_piece(g, 4);
	drop_piece(g, 1);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, draw1){
	game *g = new_game(1, 5, 5, MATRIX);
	drop_piece(g, 0);
	drop_piece(g, 1);
	cr_assert(game_outcome(g) == DRAW);
	game_free(g);
}

Test(game_outcome, draw2){
	game *g = new_game(2, 5, 5, BITS);
	drop_piece(g, 0);
	drop_piece(g, 1);
	drop_piece(g, 0);
	drop_piece(g, 1);
	cr_assert(game_outcome(g) == DRAW);
	game_free(g);
}

Test(game_outcome, white_win){
	game *g = new_game(2, 5, 5, MATRIX);
	drop_piece(g, 0);
	drop_piece(g, 0);
	drop_piece(g, 1);
	cr_assert(game_outcome(g) == BLACK_WIN);
	game_free(g);
}

Test(game_outcome, black_win){
	game *g = new_game(2, 5, 5, BITS);
	drop_piece(g, 0);
	drop_piece(g, 1);
	drop_piece(g, 3);
	drop_piece(g, 2);
	cr_assert(game_outcome(g) == WHITE_WIN);
	game_free(g);
}

Test(game_outcome, vertical_win1){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), BLACK);
	board_set(g -> b, make_pos(4, 0), BLACK);
	board_set(g -> b, make_pos(3, 0), BLACK);
	board_set(g -> b, make_pos(2, 0), BLACK);
	cr_assert(game_outcome(g) == BLACK_WIN);
	game_free(g);
}

Test(game_outcome, vertical_win2){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), WHITE);
	board_set(g -> b, make_pos(4, 0), BLACK);
	board_set(g -> b, make_pos(3, 0), BLACK);
	board_set(g -> b, make_pos(2, 0), BLACK);
	board_set(g -> b, make_pos(1, 0), BLACK);
	board_set(g -> b, make_pos(0, 0), WHITE);
	cr_assert(game_outcome(g) == BLACK_WIN);
	game_free(g);
}

Test(game_outcome, no_vertical_win1){
	game* g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), WHITE);
	board_set(g -> b, make_pos(4, 0), BLACK);
	board_set(g -> b, make_pos(3, 0), BLACK);
	board_set(g -> b, make_pos(2, 0), BLACK);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_vertical_win2){
	game* g = new_game(5, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), WHITE);
	board_set(g -> b, make_pos(4, 0), WHITE);
	board_set(g -> b, make_pos(3, 0), BLACK);
	board_set(g -> b, make_pos(2, 0), BLACK);
	board_set(g -> b, make_pos(1, 0), BLACK);
	board_set(g -> b, make_pos(0, 0), BLACK);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, horizontal_win1){
	game* g = new_game(4, 6, 6, BITS);
	board_set(g -> b, make_pos(5, 0), WHITE);
	board_set(g -> b, make_pos(5, 1), WHITE);
	board_set(g -> b, make_pos(5, 2), WHITE);
	board_set(g -> b, make_pos(5, 3), WHITE);
	cr_assert(game_outcome(g) == WHITE_WIN);
	game_free(g);
}

Test(game_outcome, horizontal_win2){
	game* g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 1), BLACK);
	board_set(g -> b, make_pos(5, 2), WHITE);
	board_set(g -> b, make_pos(5, 3), WHITE);
	board_set(g -> b, make_pos(5, 4), WHITE);
	board_set(g -> b, make_pos(5, 5), WHITE);
	cr_assert(game_outcome(g) == WHITE_WIN);
	game_free(g);
}

Test(game_outcome, no_horizontal_win1){
	game* g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 2), WHITE);
	board_set(g -> b, make_pos(5, 3), WHITE);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_horizontal_win2){
	game* g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 2), WHITE);
	board_set(g -> b, make_pos(5, 3), WHITE);
	board_set(g -> b, make_pos(5, 4), BLACK);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_horizontal_win3){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 4), WHITE);
	board_set(g -> b, make_pos(5, 5), WHITE);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, rightdiag_win1){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(1, 1), BLACK);
	board_set(g -> b, make_pos(2, 2), BLACK);
	board_set(g -> b, make_pos(3, 3), BLACK);
	cr_assert(game_outcome(g) == BLACK_WIN);
	game_free(g);
}

Test(game_outcome, rightdiag_win2){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(0, 0), WHITE);
	board_set(g -> b, make_pos(1, 1), BLACK);
	board_set(g -> b, make_pos(2, 2), BLACK);
	board_set(g -> b, make_pos(3, 3), BLACK);
	board_set(g -> b, make_pos(4, 4), WHITE);
	cr_assert(game_outcome(g) == BLACK_WIN);
	game_free(g);
}

Test(game_outcome, rightdiag_win3){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(3, 3), BLACK);
	board_set(g -> b, make_pos(4, 4), BLACK);
	board_set(g -> b, make_pos(5, 5), BLACK);
	cr_assert(game_outcome(g) == BLACK_WIN);
	game_free(g);
}

Test(game_outcome, no_rightdiag_win1){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(1, 1), BLACK);
	board_set(g -> b, make_pos(2, 2), BLACK);
	board_set(g -> b, make_pos(3, 3), BLACK);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_rightdiag_win2){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(1, 1), BLACK);
	board_set(g -> b, make_pos(2, 2), BLACK);
	board_set(g -> b, make_pos(3, 3), WHITE);
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_rightdiag_win3){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(3, 3), BLACK);
	board_set(g -> b, make_pos(4, 4), BLACK);
	board_set(g -> b, make_pos(5, 5), BLACK);
	// run ends in the corner of the board
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_rightdiag_win4){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(3, 5), BLACK);
	board_set(g -> b, make_pos(2, 4), BLACK);
	board_set(g -> b, make_pos(1, 3), BLACK);
	// run ends on the right side of the board
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_rightdiag_win5){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(3, 1), BLACK);
	board_set(g -> b, make_pos(4, 2), BLACK);
	board_set(g -> b, make_pos(5, 3), BLACK);
	// run ends on the bottom of the board
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, leftdiag_win1){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), WHITE);
	board_set(g -> b, make_pos(4, 1), WHITE);
	board_set(g -> b, make_pos(3, 2), WHITE);
	cr_assert(game_outcome(g) == WHITE_WIN);
	game_free(g);
}

Test(game_outcome, leftdiag_win2){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(4, 1), WHITE);
	board_set(g -> b, make_pos(3, 2), WHITE);
	board_set(g -> b, make_pos(2, 3), WHITE);
	cr_assert(game_outcome(g) == WHITE_WIN);
	game_free(g);
}

Test(game_outcome, leftdiag_win3){
	game *g = new_game(3, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), BLACK);
	board_set(g -> b, make_pos(4, 1), WHITE);
	board_set(g -> b, make_pos(3, 2), WHITE);
	board_set(g -> b, make_pos(2, 3), WHITE);
	cr_assert(game_outcome(g) == WHITE_WIN);
	game_free(g);
}

Test(game_outcome, no_leftdiag_win1){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), WHITE);
	board_set(g -> b, make_pos(4, 1), WHITE);
	board_set(g -> b, make_pos(3, 2), WHITE);
	// run ends in the corner of the board
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_leftdiag_win2){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 0), BLACK);
	board_set(g -> b, make_pos(4, 1), WHITE);
	board_set(g -> b, make_pos(3, 2), WHITE);
	board_set(g -> b, make_pos(2, 3), WHITE);
	// run ends on a black piece
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_leftdiag_win3){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(4, 1), WHITE);
	board_set(g -> b, make_pos(3, 2), WHITE);
	board_set(g -> b, make_pos(2, 3), WHITE);
	// run ends in the middle of the space
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_leftdiag_win4){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(5, 1), WHITE);
	board_set(g -> b, make_pos(4, 2), WHITE);
	board_set(g -> b, make_pos(3, 3), WHITE);
	// run ends on the bottom of the board
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(game_outcome, no_leftdiag_win5){
	game *g = new_game(4, 6, 6, MATRIX);
	board_set(g -> b, make_pos(3, 0), WHITE);
	board_set(g -> b, make_pos(2, 1), WHITE);
	board_set(g -> b, make_pos(1, 2), WHITE);
	// run ends on the left side of the board
	cr_assert(game_outcome(g) == IN_PROGRESS);
	game_free(g);
}

Test(disarray, stopwatch){
	unsigned char width = 8;
	unsigned char height = 255;
	game *g = new_game(4, width, height, MATRIX);
	unsigned int i, j;
	for (i = 0; i < 8; i++){
		for (j = 0; j < height; j++){
			board_set(g->b, make_pos(j, i), BLACK);
		}
	}

	struct timeval start;
	struct timeval stop;

	gettimeofday(&start, NULL);
	old_disarray(g);
	gettimeofday(&stop, NULL);

	unsigned long int t_0 = start.tv_sec * height + start.tv_usec;
	unsigned long int t_1 = stop.tv_sec * height + start.tv_usec;
	unsigned long int old_time = t_1-t_0;
	
	gettimeofday(&start, NULL);
	disarray(g);
	gettimeofday(&stop, NULL);

	t_0 = start.tv_sec * height + start.tv_usec;
	t_1 = stop.tv_sec * height + start.tv_usec;
	unsigned long int new_time = t_1-t_0;

	printf("old time: %lu\n", old_time);
	printf("new time: %lu\n", new_time);
	game_free(g);
}
