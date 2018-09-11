#include <stdlib.h>

#include "bitboard.h"
#include "chessboard.h"

Chessboard *initialise_chessboard() {
    Chessboard *chessboard;
    if ((chessboard = malloc(sizeof *chessboard)) != NULL) {
        chessboard->white_pawns = 0xFF00ULL;
        chessboard->white_rooks = 0x81ULL;
        chessboard->white_knights = 0x42ULL;
        chessboard->white_bishops = 0x24ULL;
        chessboard->white_queens = 0x8ULL;
        chessboard->white_king = 0x10ULL;
        chessboard->black_pawns = 0xFF000000000000ULL;
        chessboard->black_rooks = 0x8100000000000000ULL;
        chessboard->black_knights = 0x4200000000000000ULL;
        chessboard->black_bishops = 0x2400000000000000ULL;
        chessboard->black_queens = 0x800000000000000ULL;
        chessboard->black_king = 0x1000000000000000ULL;
        update_chessboard(chessboard);
    }
    return chessboard;
}

void update_chessboard(Chessboard *chessboard) {
    chessboard->white_pieces = chessboard->white_pawns | chessboard->white_rooks | 
                                chessboard->white_knights | chessboard->white_bishops |
                                chessboard->white_queens | chessboard->white_king;
    chessboard->black_pieces = chessboard->black_pawns | chessboard->black_rooks | 
                                    chessboard->black_knights | chessboard->black_bishops |
                                    chessboard->black_queens | chessboard->black_king;
    chessboard->all_pieces = chessboard->white_pieces | chessboard->black_pieces;
}

