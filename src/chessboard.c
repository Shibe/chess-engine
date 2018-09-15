#include <stdlib.h>

#include "bitboard.h"
#include "chessboard.h"

Chessboard *initialise_chessboard() {
	Chessboard *chessboard = malloc(sizeof(Chessboard));
	Pieces *white_pieces = malloc(sizeof(Pieces));
	Pieces *black_pieces = malloc(sizeof(Pieces));


	if(chessboard != NULL && white_pieces != NULL && black_pieces != NULL) {
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

		update_chessboard(chessboard);
	}
	return chessboard;
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