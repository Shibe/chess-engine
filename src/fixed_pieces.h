#include "bitboard.h"

Bitboard compute_king(Bitboard king_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_knight(Bitboard knight_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_pawn_attacks(int active_player, Bitboard pawn_loc, Bitboard opposing_side, Bitboard en_passant_target, Bitboard clear_file[FILE_LEN]);
Bitboard compute_pawn_moves(int active_player, Bitboard pawn_loc, Bitboard own_side, Bitboard opposing_side, Bitboard mask_rank[RANK_LEN]);
Bitboard compute_valid_pawn_moves (int active_player, Bitboard pawn_loc, Bitboard own_side, Bitboard opposing_side, Bitboard en_passant_target, Bitboard clear_file[FILE_LEN], Bitboard mask_rank[RANK_LEN]);
