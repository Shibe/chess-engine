#include "bitboard.h"

typedef struct {
    Bitboard white_pawns;
    Bitboard white_rooks;
    Bitboard white_knights;
    Bitboard white_bishops;
    Bitboard white_queens;
    Bitboard white_king;

    Bitboard black_pawns;
    Bitboard black_rooks;
    Bitboard black_knights;
    Bitboard black_bishops;
    Bitboard black_queens;
    Bitboard black_king;

    Bitboard white_pieces;
    Bitboard black_pieces;
    Bitboard all_pieces;
} Chessboard;

Chessboard *initialise_chessboard();
void update_chessboard(Chessboard *chessboard);
