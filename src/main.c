#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "chessboard.h"
#include "game.h"
#include "fen_parser.h"
#include "greatest.h"

SUITE_EXTERN(fixed_pieces_suite);
SUITE_EXTERN(sliding_pieces_suite);
SUITE_EXTERN(gamestate_suite);

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(fixed_pieces_suite);
	RUN_SUITE(sliding_pieces_suite);
	RUN_SUITE(gamestate_suite);
	GREATEST_MAIN_END();

	Chessboard *chessboard = create_chessboard();
	if (chessboard == NULL) {
		return 1;
	}
	if (argc > 1) {
		char *fen = argv[1];
		int err = parse_fen(chessboard, fen);
		if (err) {
			return 1;
		}
	} else {
		initialise_chessboard(chessboard);
	}
	game_loop(chessboard);  
	return 0;
}
