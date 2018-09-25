#include "bitboard.h"
#ifndef GAME_FILE
#define GAME_FILE

Bitboard is_checked(Bitboard king, Bitboard attacked_squares);
int move_piece(Pieces *pieces, Bitboard start, Bitboard end);
int turn(Bitboard start, Bitboard end, int active_player, Pieces *own_side, Pieces *opposing_side, Bitboard *en_passant_target);
void game_loop(Chessboard *chessboard);
Bitboard input_to_square(int rank, int file);
Bitboard compute_attacking_squares(int player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target);
int get_player_move(Bitboard *start, Bitboard *end);
int is_stalemate(Chessboard *chessboard, int player);
int calculate_stalemate(int player, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard en_passant_target);

#endif
