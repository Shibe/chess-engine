#include "bitboard.h"

typedef struct {
	Bitboard pawns;
    Bitboard rooks;
    Bitboard knights;
    Bitboard bishops;
    Bitboard queens;
    Bitboard king;
	Bitboard all;
} Pieces;

typedef struct {
	Pieces *white_pieces;
	Pieces *black_pieces;
	Bitboard all_pieces;
} Chessboard;

Chessboard *initialise_chessboard();
void clear_chessboard(Chessboard *chessboard);
void update_chessboard(Chessboard *chessboard);
void update_pieces(Pieces *pieces);
void print_chessboard(Chessboard *chessboard);
char get_symbol(Pieces *pieces, int square);