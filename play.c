#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pos.h"
#include "board.h"
#include "logic.h"

int main(int argc, char* argv[]){
	// throws errors for invalid command line args
	if (argc != 8){
		fprintf(stderr, "Main: Invalid number of command line arguments.\n");
		exit(1);
	}
	// also need to add requirement for -m or -b tag

	char height, width, run;
	enum type game_type;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-h")){
			height = atoi(argv[i+1]);
		}
		else if (!strcmp(argv[i], "-w")){
			width = atoi(argv[i+1]);
		}
		else if (!strcmp(argv[i], "-r")){
			run = atoi(argv[i+1]);
		}
		else if (!strcmp(argv[i], "-m")){
			game_type = MATRIX;
		}
		else if (!strcmp(argv[i], "-b")){
			game_type = BITS;
		}
	}
	if (height <= 0 || width <= 0 || run <= 0){
		fprintf(stderr, "Main: Invalid command line arguments");
		exit(1);
	}

	// widths that result in ? are unplayable
	if (width >= 62){
		fprintf(stderr, "Main: board width out of range");
		exit(1);
	}
	
	game *g = new_game(run, width, height, game_type);
	printf("-------- Welcome to Topsy Turvy! --------\n");
	printf("\n");
	while (game_outcome(g) == IN_PROGRESS){
		bool valid_move = false;
		while (valid_move == false){
			board_show(g -> b);
			if (g -> player == WHITES_TURN){
				printf("White: ");
			}
			else{
				printf("Black: ");
			}

			char move; // limits input to one char
			scanf("%c%*c", &move);
			if (move >= '0' && move <= '9'){
				move -= '0';
			}
			printf("\n");
			
			if (move == '!'){
				// offset
				valid_move = offset(g);
			}
			else if (move == '^'){ 
				// disarray
				disarray(g);
				valid_move = true; // disarrays are always valid
			}
			else if (move >= 'a'){
				unsigned char move_num = move - ('a' - 36);
				if (move_num >= g -> b -> width){
					valid_move = false;
				}
				else{
					valid_move = drop_piece(g, move_num);
				}
			}
			else if (move >= 'A'){
				unsigned char move_num = move - ('A' - 10);
				if (move_num >= g -> b -> width){
					valid_move = false;
				}
				else{
					valid_move = drop_piece(g, move_num);
				}
			}
			else if (10 > move && move >= 0){
				if (move >= g -> b -> width){
					valid_move = false;
				}
				else{
					valid_move = drop_piece(g, move);
				}
			}
			else {
				printf("---- invalid input ----\n\n");
				valid_move = false;
			}
		}
	}	

	printf("\n");
	board_show(g -> b);
	printf("\n");
	outcome winner = game_outcome(g);
	if (winner == WHITE_WIN){
		printf("White wins!");
	}
	else if (winner == BLACK_WIN){
		printf("Black wins!");
	}
	else{
		printf("Draw.");
	}
	printf("\n");

	return 0;
}
