#include "bitboard.h"
#ifndef GAME_FILE
#define GAME_FILE

int is_checked(Bitboard king, Bitboard attacked_squares);
int move_piece(Pieces *pieces, Bitboard start, Bitboard end);
int turn(Pieces *own_side, Pieces *opposing_side);
void game_loop(Chessboard *chessboard);
Bitboard input_to_square(int rank, int file);
Bitboard compute_attacked_squares(Pieces *own_side, Pieces *opposing_side);

#endif
