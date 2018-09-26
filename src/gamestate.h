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
Bitboard is_double_check(Bitboard king, Bitboard attacks_on_king);
Bitboard get_attacks_on_king(Bitboard king, Pieces *opponent_pieces, int player);
Bitboard compute_straight_check_lines(Bitboard king, Pieces *opponent_pieces, Bitboard clear_file[FILE_LEN]);
Bitboard compute_diagonal_check_lines(Bitboard king, Pieces *opponent_pieces, Bitboard clear_file[FILE_LEN]);
Bitboard is_checked(Bitboard king, Bitboard attacked_squares);
Bitboard compute_attacking_squares(int player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target);
int move_piece(Pieces *pieces, Bitboard start, Bitboard end);
#endif