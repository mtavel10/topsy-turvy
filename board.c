#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pos.h"
#include "board.h"

/* Helper function for bit representation testing. */
char* dec_to_bin(unsigned int n){
    char* binary = (char*)malloc(sizeof(char) * 33);
	if (binary == NULL){
		fprintf(stderr, "dec_to_bin: malloc error");
		exit(1);
	}
    unsigned int i = 0;
    while (i < 33){
        if (n % 2 != 0){
            binary[i] = '1';
        }
        else{
            binary[i] = '0';
        }
        n /= 2;
        i++;
    }
    
    // flip the string
    unsigned char j, k;
    for (j = 0, k = i - 1; j < k; j++, k--) {
        unsigned char temp = binary[j];
        binary[j] = binary[k];
        binary[k] = temp;
    }
    return binary;
}

/* Helper function that calculates the size of a bits array*/
unsigned int calc_bits_size(unsigned int width, unsigned int height){
	unsigned int size;
	unsigned int bits_needed = (width * height) * 2;
	if (bits_needed % 32 == 0){
		size = bits_needed / 32;
	}
	else{
		size = (bits_needed / 32) + 1;
	}
	return size;
}

/* Creates a fully-empty board of the desired size.*/
board* board_new(unsigned int width, unsigned int height, enum type type){
	board_rep u;
	if (type == BITS){
		unsigned int size = calc_bits_size(width, height);
		unsigned int* bits = (unsigned int*)malloc(sizeof
							(unsigned int) * size);
		if (bits == NULL){
			fprintf(stderr, "board_new: bits malloc error");
			exit(1);
		}
		for (int i = 0; i < size; i++){
			bits[i] = 0;
		}
		u.bits = bits;
	}
	else{
		if (width <= 0 || height <= 0){
			fprintf(stderr, "board_new: invalid dimensions\n");
			exit(1);
		}
		// Creates the matrix
		cell** matrix = (cell**)malloc(sizeof(cell*) * height);
		if (matrix == NULL){
			fprintf(stderr, "board_new: matrix malloc error");
			exit(1);
		}
		unsigned int i;
		for (i = 0; i < height; i++){
			matrix[i] = (cell*)malloc(sizeof(cell) * width);
			if (matrix[i] == NULL){
				fprintf(stderr, "board_new: matrix row malloc error");
				exit(1);
			}
			unsigned int j;
			for (j = 0; j < width; j++){
				matrix[i][j] = EMPTY;
			}
		} 
		u.matrix = matrix;
	}

	board* b = (board*)malloc(sizeof(board));
	if (b == NULL){
			fprintf(stderr, "board_new: board malloc error");
			exit(1);
	}
	b -> width = width;
	b -> height = height;
	b -> type = type;
	b -> u = u;
	
	return b;
}

/* Fully frees the board and its internal representation.
 * For now, raises an error if BITS representation is used. */
void board_free(board* b){
	if (b -> type == BITS){
		free(b -> u.bits);
		free(b);
	}
	else{
		unsigned int i;
		// Free each row
		for (i = 0; i < b -> height; i++){
			free(b -> u.matrix[i]);
		}
		// Free the columns
		free(b -> u.matrix);
		
		free(b);
	}
}

/* Helper function to print the headers of the board in the correct format*/
char create_header_char(int j){
	if (j >= 62){
		return '?';
	}
	else if (j >= 36){
		return j + ('a' - 36);
	}
	else if (j >= 10){
		return j + ('A' - 10);
	}
	else {
		return j + '0';
	}
}


/* Prints the board to the screen with row and column headers.
 * Blank horizonal and vertical lines separate headers from board.
 * Headers are represented as digits 0-9, A-Z, a-z, then ?s */
void board_show(board* b){
	char column_header[(b -> width) + 1];

	for (int i = 0; i < b -> height; i++){
		// adds to the row headers
		printf("%c ", create_header_char(i));
		for (int j = 0; j < b -> width; j++){
			// adds to the current row
			cell curr_cell = board_get(b, make_pos(i, j));
			if (curr_cell == WHITE){
				printf("o");
			}
			else if (curr_cell == BLACK){
				printf("*");
			}
			else if (curr_cell == EMPTY){
				printf(".");
			}
			
			// Creates the column_header string
			if (i == 0){
				column_header[j] = 
						create_header_char(j);
			}
		}
		printf("\n");
	}
	printf("\n");
	column_header[(b -> width)] = '\0';
	printf("  %s\n", column_header);
	printf("\n");
}

/* Retrieves a cell within the board.
 * Raises error if out of bounds
 * For now, raises an error if BITS reprsentation is used */
cell board_get(board* b, pos p){
	if (b -> type == BITS){
		unsigned int elem = (p.r * b -> width) + p.c;
		unsigned int i = elem / 16; // bits array index
		unsigned int j = elem % 16; // where in the bits elem is this stored

		// gets the piece at this location
		unsigned int cell_bits = (b -> u.bits[i] & 0x3 << (j * 2)) >> (j * 2);
		
		if (cell_bits == 1){
			return BLACK;
		}
		else if (cell_bits == 2){
			return WHITE;
		}
		else if (cell_bits == 0){
			return EMPTY;
		}
		else{
			fprintf(stderr, "board_get: did bitwise things wrong");
			exit(1);
		}
	}
	if (p.r >= b -> height || p.c >= b -> width){
		printf("%u, %u\n", p.r, p.c);
		fprintf(stderr, "board_get: position out of bounds\n");
		exit(1);
	}
	return b -> u.matrix[p.r][p.c];
}

/* Modifies a cell within the board.
 * Raises error if out of bounds
 * For now, raises an eror if BITS representation is used */
void board_set(board* b, pos p, cell c){
	if (p.r >= b -> height || p.c >= b -> width){
		fprintf(stderr, "board_set: position (%u, %u) out of bounds", p.r, p.c);
		exit(1);
	}
	if (b -> type == BITS){
		unsigned int elem = (p.r * b -> width) + p.c;
		unsigned int i = elem / 16; // bits array index
		unsigned int j = elem % 16; // where in the bits elem is this stored
		
		// sets the piece at this location
		if (c == WHITE){
			b -> u.bits[i] |= (0x2 << (j * 2));
		}
		else if (c == BLACK){
			b -> u.bits[i] |= (0x1 << (j * 2));
		}
		else{ // when c is empty
			b -> u.bits[i] &= ~(0x3 << (j * 2));
		}
	}
	else{
	b -> u.matrix[p.r][p.c] = c;
	}
}
