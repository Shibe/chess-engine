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
		return is_stalemate(player_pieces->king, player_pieces->all, opponent_attacks, player_attacks);
	}

	return is_mate(player, player_pieces, opponent_pieces, player_attacks, opponent_attacks, chessboard);
}

int is_stalemate(Bitboard king, Bitboard own_side, Bitboard opponent_attacks, Bitboard player_attacks) {
	if (king & opponent_attacks) { // King is in check
		return GS_ACTIVE;
	}

	Bitboard king_moves = compute_king(king, own_side, clear_file);
	if (king_moves & ~opponent_attacks || player_attacks & ~king_moves) { // Still valid moves left
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

	// Check all possible moves, see if any doesn't result in check.
	for (Bitboard p = 1ULL; p; p <<= 1) {
		if (p & ~player_pieces->all)
		{
			continue;
		}

		Bitboard valid_positions;
		if (p & player_pieces->pawns) {		
			valid_positions = compute_valid_pawn_moves(player, player_pieces->pawns & p, player_pieces->all, opponent_pieces->all, chessboard->en_passant_target, clear_file, mask_rank);		
		} else if (p & player_pieces->rooks) {
			valid_positions = compute_rook(player_pieces->rooks & p, player_pieces->all, opponent_pieces->all, clear_file);
		} else if (p & player_pieces->knights) {
			valid_positions = compute_knight(player_pieces->knights & p, player_pieces->all, clear_file);
		} else if (p & player_pieces->bishops) {
			valid_positions = compute_bishop(player_pieces->bishops & p, player_pieces->all, opponent_pieces->all, clear_file);
		} else if (p & player_pieces->queens) {
			valid_positions = compute_queen(player_pieces->queens & p, player_pieces->all, opponent_pieces->all, clear_file);
		} else if (p & player_pieces->king) {
			valid_positions = compute_king(player_pieces->king & p, player_pieces->all, clear_file);
		}

		for (Bitboard j = 1ULL; j; j <<= 1) {
			if (j & ~valid_positions) {
				continue;
			}
			int failure = attempt_move(player, p, j, chessboard);
			if (!failure) {
				return 0;
			}
		}
	}
	return 1;
}

int attempt_move(int player, Bitboard start, Bitboard end, Chessboard *chessboard) {
	Pieces player_pieces_cpy, opponent_pieces_cpy;

	if (player == WHITE) {
		player_pieces_cpy = *chessboard->white_pieces;
		opponent_pieces_cpy = *chessboard->black_pieces;
	} else {
		player_pieces_cpy = *chessboard->black_pieces;
		opponent_pieces_cpy = *chessboard->white_pieces;
	}

	attack_square(player, start, end, &player_pieces_cpy, &opponent_pieces_cpy, chessboard->en_passant_target);

	Bitboard attack_lines = compute_attacking_squares(!player, &opponent_pieces_cpy, &player_pieces_cpy, chessboard->en_passant_target);
	Bitboard checked = is_checked(player_pieces_cpy.king, attack_lines);
	if (!checked) {
		return 0;
	}
	return -1;
}

int attack_square(int player, Bitboard start, Bitboard end, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard en_passant_target) {
	int err;
	
	err = move_piece(player_pieces, start, end);
	if (err) {
		return err;
	}

	// En passant take
	if (end == en_passant_target && end & player_pieces->pawns) {
		if (player == WHITE)
			end >>= 8;
		else
			end <<= 8;
	}
	err = move_piece(opponent_pieces, end, 0x0ULL);
	return err;
}

Bitboard is_checked(Bitboard king, Bitboard attacked_squares) {
	return king & attacked_squares;
}

Bitboard compute_attacking_squares(int player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target) {
	Bitboard pawns = compute_pawn_attacks(player, own_side->pawns, opposing_side->all, en_passant_target, clear_file);	
	Bitboard rooks = compute_rook(own_side->rooks, own_side->all, opposing_side->all, clear_file);
	Bitboard knights = compute_knight(own_side->knights, own_side->all, clear_file);
	Bitboard bishops = compute_bishop(own_side->bishops, own_side->all, opposing_side->all, clear_file);
	Bitboard queens = compute_queen(own_side->queens, own_side->all, opposing_side->all, clear_file);
	Bitboard king = compute_king(own_side->king, own_side->all, clear_file);
	return pawns | rooks | knights | bishops | queens | king;
}

int move_piece(Pieces *pieces, Bitboard start, Bitboard end) {
	if (start & pieces->pawns) {
		if ((mask_rank[RANK_8] & end) || (mask_rank[RANK_1] & end)) {
			promote_pawn(pieces, start, end);
			return 0;
		}
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
		return -1;
	}
	update_pieces(pieces);
	return 0;
}