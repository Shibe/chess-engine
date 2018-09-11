Bitboard compute_valid_attack(Bitboard loc, char shift_amount, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_queen(Bitboard loc, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_bishop(Bitboard loc, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]);
Bitboard compute_rook(Bitboard loc, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]);