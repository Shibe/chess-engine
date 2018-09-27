#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "game.h"
#ifndef GAMESTATE_FILE
#define GAMESTATE_FILE

enum gamestate { GS_ACTIVE, GS_MATE, GS_STALEMATE };

int get_gamestate(Chessboard *chessboard, int player);
int is_stalemate(Bitboard king, Bitboard opponent_attacks, Bitboard player_attacks);
int is_mate(int player, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard player_attacks, Bitboard opponent_attacks, Chessboard *chessboard);
int attempt_move(int player, Bitboard start, Bitboard end, Chessboard *chessboard);
int attack_square(int player, Bitboard start, Bitboard end, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard en_passant_target);
Bitboard is_checked(Bitboard king, Bitboard attacked_squares);
Bitboard compute_attacking_squares(int player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target);
int move_piece(Pieces *pieces, Bitboard start, Bitboard end);

#endif