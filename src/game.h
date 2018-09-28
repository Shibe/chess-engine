#include "bitboard.h"
#include "chessboard.h"
#ifndef GAME_FILE
#define GAME_FILE

int turn(Bitboard start, Bitboard end, int active_player, Pieces *own_side, Pieces *opposing_side, Bitboard *en_passant_target);
void game_loop(Chessboard *chessboard);
Bitboard input_to_square(int rank, int file);
int get_player_move(Bitboard *start, Bitboard *end);

#endif
