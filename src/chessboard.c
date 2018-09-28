#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "bitboard.h"
#include "chessboard.h"

#ifdef _WIN32
	#define COLOR_RED ""
	#define COLOR_GREEN ""
	#define RESET ""
#else
	#define COLOR_RED "\x1b[31m"
	#define COLOR_GREEN "\x1b[32m"
	#define RESET "\x1b[0m"
#endif

Chessboard *create_chessboard() {
	Chessboard *chessboard = malloc(sizeof(Chessboard));
	if(chessboard == NULL) {
		puts("Couldn't allocate chessboard.");
		return NULL;
	}
	Pieces *white_pieces = malloc(sizeof(Pieces));
	if(white_pieces == NULL) {
		puts("Couldn't allocate white pieces.");
		free(chessboard);
		return NULL;
	}
	Pieces *black_pieces = malloc(sizeof(Pieces));
	if(black_pieces == NULL) {
		puts("Couldn't allocate black pieces.");
		free(chessboard);
		free(white_pieces);
		return NULL;
	}
	chessboard->white_pieces = white_pieces;
	chessboard->black_pieces = black_pieces;
	clear_chessboard(chessboard);
	return chessboard;
}

void initialise_chessboard(Chessboard *chessboard) {
	Pieces *white_pieces = chessboard->white_pieces;
	Pieces *black_pieces = chessboard->black_pieces;
	white_pieces->pawns = 0xFF00ULL;
	white_pieces->rooks = 0x81ULL;
	white_pieces->knights = 0x42ULL;
	white_pieces->bishops = 0x24ULL;
	white_pieces->queens = 0x8ULL;
	white_pieces->king = 0x10ULL;
	black_pieces->pawns = 0xFF000000000000ULL;
	black_pieces->rooks = 0x8100000000000000ULL;
	black_pieces->knights = 0x4200000000000000ULL;
	black_pieces->bishops = 0x2400000000000000ULL;
	black_pieces->queens = 0x800000000000000ULL;
	black_pieces->king = 0x1000000000000000ULL;
	chessboard->black_pieces = black_pieces;
	chessboard->white_pieces = white_pieces;
	chessboard->active_color = WHITE;
	chessboard->castle_white = LONG | SHORT;
	chessboard->castle_black = LONG | SHORT;
	chessboard->en_passant_target = 0x0ULL;
	chessboard->halfmove_clock = 0;
	chessboard->fullmove_number = 0;

	update_chessboard(chessboard);
}

void clear_chessboard(Chessboard *chessboard) {
	Pieces *white_pieces = chessboard->white_pieces;
	Pieces *black_pieces = chessboard->black_pieces;
	white_pieces->pawns = 0x0ULL;
	white_pieces->rooks = 0x0ULL;
	white_pieces->knights = 0x0ULL;
	white_pieces->bishops = 0x0ULL;
	white_pieces->queens = 0x0ULL;
	white_pieces->king = 0x0ULL;
	black_pieces->pawns = 0x0ULL;
	black_pieces->rooks = 0x0ULL;
	black_pieces->knights = 0x0ULL;
	black_pieces->bishops = 0x0ULL;
	black_pieces->queens = 0x0ULL;
	black_pieces->king = 0x0ULL;
	chessboard->active_color = WHITE;
	chessboard->castle_white = NONE;
	chessboard->castle_black = NONE;
	chessboard->en_passant_target = 0x0ULL;
	chessboard->halfmove_clock = 0;
	chessboard->fullmove_number = 0;
	update_chessboard(chessboard);
}

void update_chessboard(Chessboard *chessboard) {
	update_pieces(chessboard->white_pieces);
	update_pieces(chessboard->black_pieces);
	chessboard->all_pieces = chessboard->black_pieces->all | chessboard->white_pieces->all;
}

void update_pieces(Pieces *pieces) {
	pieces->all = pieces->pawns | pieces-> rooks | pieces->knights |
		pieces->bishops | pieces->queens | pieces->king;
}

static void print_files() {
	char file_symbols[] = "ABCDEFGH";
	printf("  ");
	for (int i = 0; i < sizeof(file_symbols); i++) {
		printf("%c ", file_symbols[i]);
	}
	puts("");
}

void print_chessboard(Chessboard *chessboard) {
	print_files();

	for (int i = RANK_8; i >= RANK_1; i--) {
		printf("%d ", i + 1);
		for (int j = FILE_A; j <= FILE_H; j++) {
			char c;
			c = get_symbol(chessboard->black_pieces, j+8*i, BLACK);
			if (c != '.') {
				printf(COLOR_RED "%c " RESET, c);
			} else {
				c = get_symbol(chessboard->white_pieces, j+8*i, WHITE);
				if (c == '.') {
					printf("%c ", c);
				} else {
					printf(COLOR_GREEN "%c " RESET, c);
				}
			}
		}
		printf("%d", i + 1);
		printf ("\n");
	}

	print_files();
}

char get_symbol(Pieces *pieces, int square, int active_player) {
	Bitboard mask = 1ULL << square;
	char token;
	if (pieces->pawns & mask) {
		token = 'p';
	} else if (pieces->rooks & mask) {
		token =  'r';
	} else if (pieces->knights & mask) {
		token = 'n';
	} else if (pieces->bishops & mask) {
		token = 'b';
	} else if (pieces->queens & mask) {
		token = 'q';
	} else if (pieces->king & mask) {
		token = 'k';
	} else {
		token = '.';
	}
	if (active_player == BLACK) {
		return token;
	} else {
		return toupper(token);
	}
}
