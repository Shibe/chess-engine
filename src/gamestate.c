#include "gamestate.h"

int get_gamestate(Chessboard *chessboard, int player) {
	Pieces *player_pieces;
	Pieces *opponent_pieces;
	Bitboard player_attacks;
	Bitboard opponent_attacks;

	Bitboard white_attacks = compute_attacking_squares(WHITE, chessboard->white_pieces, chessboard->black_pieces, chessboard->en_passant_target);
	Bitboard black_attacks = compute_attacking_squares(BLACK, chessboard->black_pieces, chessboard->white_pieces, chessboard->en_passant_target);

	if (player == WHITE) {
		player_pieces = chessboard->white_pieces;
		opponent_pieces = chessboard->black_pieces;
		player_attacks = white_attacks;
		opponent_attacks = black_attacks; 
	} else {
		player_pieces = chessboard->black_pieces;
		opponent_pieces = chessboard->white_pieces;
		player_attacks = black_attacks;
		opponent_attacks = white_attacks; 
	}
	
	Bitboard in_check = is_checked(player_pieces->king, opponent_attacks);
	if (!in_check) {
		return is_stalemate(player_pieces->king, opponent_attacks, player_attacks);
	}

	return is_mate(player, player_pieces, opponent_pieces, player_attacks, opponent_attacks, chessboard);
}

int is_stalemate(Bitboard king, Bitboard opponent_attacks, Bitboard player_attacks) {
	if (king & opponent_attacks) { // King is in check
		return GS_ACTIVE;
	}
	Bitboard king_moves = compute_king(king, 0x0ULL, clear_file) & ~opponent_attacks;
	if (king_moves || player_attacks) { // Still valid moves left
		return GS_ACTIVE;
	}
	return GS_STALEMATE;
}

int is_mate(int player, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard player_attacks, Bitboard opponent_attacks, Chessboard *chessboard) {
	// Is king checked.
	Bitboard checked = is_checked(player_pieces->king, opponent_attacks);		
	if (!checked) {
		return GS_ACTIVE;
	}

	// Can king move out of check
	Bitboard valid_king_moves = compute_king(player_pieces->king, player_pieces->all, clear_file) & ~opponent_attacks & ~opponent_pieces->all;
	if (valid_king_moves) {
		return GS_ACTIVE;
	}

	Bitboard check_lines = get_attacks_on_king(player_pieces->king, player_pieces, opponent_pieces, player);

	// Is double checked
	int double_checked = is_double_check(player_pieces->king, check_lines);
    if (double_checked) {
        return GS_MATE;
    }
    
	// Can be blocked by a piece
	Bitboard possible_block_or_capture = player_attacks & check_lines;
	if (!possible_block_or_capture) {
		return GS_MATE;
	}
	// Check if intercept is pinned.
	for (Bitboard a = 1ULL; a; a = a << 1) {
		if (player_pieces->pawns & a) {
			Bitboard intercepts = compute_pawn(player, player_pieces->pawns & a, player_pieces->all, opponent_pieces->all, chessboard->en_passant_target, mask_rank, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is divisible by 2 (only 1 bit is set to 1)
				Bitboard copy = player_pieces->pawns;
				player_pieces->pawns = (player_pieces->pawns & ~a) | intercepts;
				update_chessboard(chessboard);
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->pawns = copy;
				update_chessboard(chessboard);
				if (!checked) {
					return 0;	
				}
			}
		}
		if (player_pieces->queens & a) {
			Bitboard intercepts = compute_queen(player_pieces->queens & a, player_pieces->all, opponent_pieces->all, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is divisible by 2 (only 1 bit is set to 1)
				Bitboard copy = player_pieces->queens;
				player_pieces->queens = (player_pieces->queens & ~a) | intercepts;
				update_chessboard(chessboard);
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->queens = copy;
				update_chessboard(chessboard);
				if (!checked) {
					return 0;	
				}
			}
		}
		if (player_pieces->rooks & a) {
			Bitboard intercepts = compute_rook(player_pieces->rooks & a, player_pieces->all, opponent_pieces->all, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is divisible by 2 (only 1 bit is set to 1)
				Bitboard copy = player_pieces->rooks;
				player_pieces->rooks = (player_pieces->rooks & ~a) | intercepts;
				update_chessboard(chessboard);
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->rooks = copy;
				update_chessboard(chessboard);
				if (!checked) {
					return 0;	
				}
			}
		}
		if (player_pieces->bishops & a) {
			Bitboard intercepts = compute_bishop(player_pieces->bishops & a, player_pieces->all, opponent_pieces->all, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is divisible by 2 (only 1 bit is set to 1)
				Bitboard copy = player_pieces->bishops;
				player_pieces->bishops = (player_pieces->bishops & ~a) | intercepts;\
				update_chessboard(chessboard);
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->bishops = copy;
				update_chessboard(chessboard);
				if (!checked) {
					return 0;	
				}
			}
		}
		if (player_pieces->knights & a) {
			Bitboard intercepts = compute_knight(player_pieces->knights & a, player_pieces->all, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is power of 2 (only 1 bit is set to 1)
				Bitboard copy = player_pieces->knights;
				player_pieces->knights = (player_pieces->knights & ~a) | intercepts;
				update_chessboard(chessboard);
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->knights = copy;
				update_chessboard(chessboard);
				if (!checked) {
					return 0;	
				}
			}
		}
		if (player_pieces->king & a) {
			Bitboard intercepts = compute_king(player_pieces->king & a, player_pieces->all, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is power of 2 (only 1 bit is set to 1)
				Pieces opponent_copy = *opponent_pieces;
				Pieces player_copy = *player_pieces;
				move_piece(&player_copy, a, intercepts);
				move_piece(&opponent_copy, intercepts, 0x0ULL);
				update_pieces(&opponent_copy);
				update_pieces(&player_copy);
				Bitboard checked = is_checked(player_copy.king, compute_attacking_squares(!player, &opponent_copy, &player_copy, chessboard->en_passant_target));
				if (!checked) {
					return 0;	
				}
			}
		}
	}
	return 1;
}

Bitboard is_checked(Bitboard king, Bitboard attacked_squares) {
	return king & attacked_squares;
}
 
Bitboard is_double_check(Bitboard king, Bitboard attacks_on_king) {
    Bitboard king_attacks = compute_king(king, 0x0ULL, clear_file);
	Bitboard knight_attacks = compute_knight(king, 0x0ULL, clear_file);
	Bitboard king_mask = king_attacks | knight_attacks;
    Bitboard checks = king_mask & attacks_on_king;
    return checks && (checks & (checks - 1));
}

Bitboard get_attacks_on_king(Bitboard king, Pieces *own_pieces, Pieces *opponent_pieces, int player) {
    Bitboard horizontal_check_lines = compute_straight_check_lines(king, own_pieces, opponent_pieces, clear_file);
	Bitboard diagonal_check_lines = compute_diagonal_check_lines(king, own_pieces, opponent_pieces, clear_file);
	Bitboard knight_checks = compute_knight(king, 0x0ULL, clear_file) & opponent_pieces->knights;
	Bitboard pawn_checks = compute_pawn(player, king, 0x0ULL, opponent_pieces->all, 0x0ULL, mask_rank, clear_file) & opponent_pieces->pawns;
    return horizontal_check_lines | diagonal_check_lines | knight_checks | pawn_checks;
}

Bitboard compute_straight_check_lines(Bitboard king, Pieces *own_pieces, Pieces *opponent_pieces, Bitboard clear_file[FILE_LEN]) {
	Bitboard straight_sliders = opponent_pieces->queens | opponent_pieces->rooks;
	Bitboard check_lines = 0x0;
	Bitboard p, line;

	line = 0x0;
	for (p = king << 1; p & clear_file[FILE_A] & ~own_pieces->all; p = p << 1) {
		line |= p;
        if (p & straight_sliders) {
            check_lines |= line;
            break;
        }
	}

	line = 0x0;
	for (p = king >> 1; p & clear_file[FILE_H] & ~own_pieces->all; p = p >> 1) {
		line |= p;
        if (p & straight_sliders) {
            check_lines |= line;
            break;
        }
	}

	line = 0x0;
	for (p = king << 8; p & ~own_pieces->all; p = p << 8) {
		line |= p;
        if (p & straight_sliders) {
            check_lines |= line;
            break;
        }
	}

	line = 0x0;
	for (p = king >> 8; p & ~own_pieces->all; p = p >> 8) {
		line |= p;
        if (p & straight_sliders) {
            check_lines |= line;
            break;
        }
	}

    return check_lines;
}

Bitboard compute_diagonal_check_lines(Bitboard king, Pieces *own_pieces, Pieces *opponent_pieces, Bitboard clear_file[FILE_LEN]) {
	Bitboard diagonal_sliders = opponent_pieces->queens | opponent_pieces->bishops;
	Bitboard check_lines = 0x0;
	Bitboard p, line;

	line = 0x0;
	for (p = king << 7; p & clear_file[FILE_H] & ~own_pieces->all; p = p << 7) {
		line |= p;
        if (p & diagonal_sliders) {
            check_lines |= line;
            break;
        }
	}

    line = 0x0;
	for (p = king >> 7; p & clear_file[FILE_A] & ~own_pieces->all; p = p >> 7) {
		line |= p;
        if (p & diagonal_sliders) {
            check_lines |= line;
            break;
        }
	}

    line = 0x0;
	for (p = king << 9; p & clear_file[FILE_A] & ~own_pieces->all; p = p << 9) {
		line |= p;
        if (p & diagonal_sliders) {
            check_lines |= line;
            break;
        }
	}

    line = 0x0;
	for (p = king >> 9; p & clear_file[FILE_H] & ~own_pieces->all; p = p >> 9) {
		line |= p;
        if (p & diagonal_sliders) {
            check_lines |= line;
            break;
        }
	}

    return check_lines;
}

Bitboard compute_attacking_squares(int player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target) {
	Bitboard pawns = compute_pawn(player, own_side->pawns, own_side->all, opposing_side->all, en_passant_target, mask_rank, clear_file);
	Bitboard rooks = compute_rook(own_side->rooks, own_side->all, opposing_side->all, clear_file);
	Bitboard knights = compute_knight(own_side->knights, own_side->all, clear_file);
	Bitboard bishops = compute_bishop(own_side->bishops, own_side->all, opposing_side->all, clear_file);
	Bitboard queens = compute_queen(own_side->queens, own_side->all, opposing_side->all, clear_file);
	Bitboard king = compute_king(own_side->king, own_side->all, clear_file);
	return pawns | rooks | knights | bishops | queens | king;
}

int move_piece(Pieces *pieces, Bitboard start, Bitboard end) {
	if (start & pieces->pawns) {
		pieces->pawns = (pieces->pawns & ~start) | end;
	} else if (start & pieces->rooks) {
		pieces->rooks = (pieces->rooks & ~start) | end;
	} else if (start & pieces->knights) {
		pieces->knights = (pieces->knights & ~start) | end;
	} else if (start & pieces->bishops) {
		pieces->bishops = (pieces->bishops & ~start) | end;
	} else if (start & pieces->queens) {
		pieces->queens = (pieces->queens & ~start) | end;
	} else if (start & pieces->king) {
		pieces->king = (pieces->king & ~start) | end;
	} else {
		return 0;
	}
	return 1;
}