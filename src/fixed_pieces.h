#include "bitboard.h"

Bitboard compute_king(Bitboard king_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_knight(Bitboard knight_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_pawn(int turn, Bitboard pawn_loc, Bitboard own_side, Bitboard opposing_side, Bitboard mask_rank[RANK_LEN], Bitboard clear_file[FILE_LEN]);