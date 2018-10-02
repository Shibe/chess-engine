#include "bitboard.h"
#include "chessboard.h"

Bitboard compute_king(Bitboard king_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_castling(int active_player, Chessboard *chessboard);
Bitboard compute_knight(Bitboard knight_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_pawn(int active_player, Bitboard pawn_loc, Bitboard own_side, Bitboard opposing_side, Bitboard en_passant_target, Bitboard mask_rank[RANK_LEN], Bitboard clear_file[FILE_LEN]);