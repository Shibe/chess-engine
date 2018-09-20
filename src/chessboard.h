#include "bitboard.h"

enum player_color { WHITE, BLACK };
enum castling { NONE = 0, SHORT = 1, LONG = 2 };

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
    int active_color;
    int castle_white;
    int castle_black;
    Bitboard en_passant_target;
    int halfmove_clock;
    int fullmove_number;
} Chessboard;

Chessboard *create_chessboard();
void initialise_chessboard(Chessboard *chessboard);
void clear_chessboard(Chessboard *chessboard);
void update_chessboard(Chessboard *chessboard);
void update_pieces(Pieces *pieces);
void print_chessboard(Chessboard *chessboard);
char get_symbol(Pieces *pieces, int square, int active_player);
