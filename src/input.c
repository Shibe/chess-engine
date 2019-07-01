#include "input.h"
#include "bitboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // for tolower

int read_move(int *start_file, int *start_rank, int *end_file, int *end_rank) {
	char raw_move[6];
	fflush(stdin);
	fgets(raw_move, 6, stdin);
	return parse_file(raw_move[0], start_file) ||
		parse_rank(raw_move[1], start_rank) ||
		parse_file(raw_move[2], end_file) ||
		parse_rank(raw_move[3], end_rank);
}

int parse_file(char c, int *file) {
	switch(tolower(c)) {
		case 'a':
			*file = 0;
			return 0;
		case 'b':
			*file = 1;
			return 0;
		case 'c':
			*file = 2;
			return 0;
		case 'd':
			*file = 3;
			return 0;
		case 'e':
			*file = 4;
			return 0;
		case 'f':
			*file = 5;
			return 0;
		case 'g':
			*file = 6;
			return 0;
		case 'h':
			*file = 7;
			return 0;
		default:
			return -1;
	}
}

int parse_rank(char c, int *rank) {
	int parsed = 0;
	switch(c) {
		case '1':
			parsed = 1;
			break;
		case '2':
			parsed = 2;
			break;
		case '3':
			parsed = 3;
			break;
		case '4':
			parsed = 4;
			break;
		case '5':
			parsed = 5;
			break;
		case '6':
			parsed = 6;
			break;
		case '7':
			parsed = 7;
			break;
		case '8':
			parsed = 8;
			break;
	}
	if(!parsed)
		return -1;

	*rank = parsed - 1;
		return 0;
}