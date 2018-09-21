#include "input.h"
#include <stdio.h>
#include <stdlib.h>

int read_move(int *start_file, int *start_rank, int *end_file, int *end_rank) {
	char raw_move[6];
	fflush(stdin);
	fgets(raw_move, 6, stdin);
	return parse_file(raw_move[0], start_file) &&
		parse_rank(raw_move[1], start_rank) &&
		parse_file(raw_move[2], end_file) &&
		parse_rank(raw_move[3], end_rank);
}

int parse_file(char c, int *file) {
	switch(c) {
		case 'a':
			*file = 0;
			return 1;
		case 'b':
			*file = 1;
			return 1;
		case 'c':
			*file = 2;
			return 1;
		case 'd':
			*file = 3;
			return 1;
		case 'e':
			*file = 4;
			return 1;
		case 'f':
			*file = 5;
			return 1;
		case 'g':
			*file = 6;
			return 1;
		case 'h':
			*file = 7;
			return 1;
		default:
			return 0;
	}
}

int parse_rank(char c, int *rank) {
	int parsed = atoi(&c);
	if (parsed >= 1 && parsed <= 8) {
		*rank = parsed - 1;
		return 1;
	}
	else {
		return 0;
	}
}