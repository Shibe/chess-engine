#include "bitboard.h"
#include "chessboard.h"
#ifndef GAME_FILE
#define GAME_FILE

int turn(Chessboard *chessboard, Bitboard start, Bitboard end, Pieces *own_side, Pieces *opposing_side);
void game_loop(Chessboard *chessboard);
Bitboard input_to_square(int rank, int file);
int get_player_move(Bitboard *start, Bitboard *end);

#endif
