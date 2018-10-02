#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "chessboard.h"
#include "game.h"
#include "fen_parser.h"

int main(int argc, char *argv[]) {
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
