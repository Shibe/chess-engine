#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "input.h"
#include "game.h"
#include <stdio.h>


void game_loop(Chessboard *chessboard) {
	while (1) {
		int active_player = chessboard->active_color;
		print_chessboard(chessboard);

		int game_state = is_mate(chessboard, active_player);
		if (game_state == 2) {
			puts("Stalemate");
			return;
		} else if (game_state == 1) {
			puts("Checkmate");
			return;
		}

		Bitboard start, end;
		Pieces *own_pieces, *opposing_pieces;

		if (active_player == WHITE) {
			own_pieces = chessboard->white_pieces;
			opposing_pieces = chessboard->black_pieces;
			printf("%s", "White player enter your move.\n");
		} else {
			own_pieces = chessboard->black_pieces;
			opposing_pieces = chessboard->white_pieces;
			printf("%s", "Black player enter your move.\n");	
		}

		int error = get_player_move(&start, &end);
		if (error) {
			puts("Invalid move given. Use notation like 'a3a4' or 'g7g1'.");
			continue;
		}

		int success = turn(start, end, active_player, own_pieces, opposing_pieces, &chessboard->en_passant_target);
		if (success) {
			update_chessboard(chessboard);
			chessboard->active_color = !active_player;
		}
	}
}

int get_player_move(Bitboard *start, Bitboard *end) {
	int start_file, start_rank, end_file, end_rank;
	int correct_move = read_move(&start_file, &start_rank, &end_file, &end_rank);
	if (!correct_move) {
		return 1;
	}
	*start = input_to_square(start_rank, start_file);
	*end = input_to_square(end_rank ,end_file);
	return 0;
}

int turn(Bitboard start, Bitboard end, int active_player, Pieces *own_side, Pieces *opposing_side, Bitboard *en_passant_target) {
	Bitboard is_valid_move = 0x0ULL;

	if (!(own_side->all & start)) {
		return 0;
	}
	
	if (start & own_side->pawns) {
		is_valid_move = end & compute_pawn(active_player, own_side->pawns & start, own_side->all, opposing_side->all, *en_passant_target, mask_rank, clear_file);
	} else if (start & own_side->rooks) {
		is_valid_move = end & compute_rook(own_side->rooks & start, own_side->all, opposing_side->all, clear_file);
	} else if (start & own_side->knights) {
		is_valid_move = end & compute_knight(own_side->knights & start, own_side->all, clear_file);
	} else if (start & own_side->bishops) {
		is_valid_move = end & compute_bishop(own_side->bishops & start, own_side->all, opposing_side->all, clear_file);
	} else if (start & own_side->queens) {
		is_valid_move = end & compute_queen(own_side->queens & start, own_side->all, opposing_side->all, clear_file);
	} else if (start & own_side->king) {
		is_valid_move = end & compute_king(own_side->king & start, own_side->all, clear_file);
	}

	if (!is_valid_move) {
		return 0;
	}
	
	if (start & own_side->pawns) {
		if (active_player == WHITE && end == (start << 16)) {
			*en_passant_target = start << 8;
		} else if (end == (start >> 16)) {
			*en_passant_target = start >> 8;
		} else if (end == *en_passant_target) {
			move_piece(own_side, start, end);
			if (active_player == WHITE) {
				end = *en_passant_target >> 8;
			} else {
				end = *en_passant_target << 8;
			}
			move_piece(opposing_side, end, 0x0ULL);
			*en_passant_target = 0x0ULL;
			return 1;
		} else {
			*en_passant_target = 0x0ULL;
		}
	} else {
		*en_passant_target = 0x0ULL;
	}

	move_piece(own_side, start, end);
	update_pieces(own_side);
	Bitboard in_check = is_checked(own_side->king, compute_attacking_squares(!active_player, opposing_side, own_side, en_passant_target));
	if (in_check) { // Moving causes king to be checked. Not allowed.
		move_piece(own_side, end, start);
		update_pieces(own_side);
		return 0;
	}
	move_piece(opposing_side, end, 0x0ULL);
	return 1;
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

Bitboard input_to_square(int rank, int file) {
	Bitboard location = 0x00000000000001ULL; // Bottom left
	int shift_amount = rank * 8 + file;
	location = location << shift_amount;
	return location;
}

Bitboard compute_attacking_squares(int player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target) {
	Bitboard pawns = compute_pawn(player, own_side->pawns, own_side->all, en_passant_target, own_side->all, mask_rank, clear_file); //TODO: Not every valid move is an attacked square, should be split up.
	Bitboard rooks = compute_rook(own_side->rooks, own_side->all, opposing_side->all, clear_file);
	Bitboard knights = compute_knight(own_side->knights, own_side->all, clear_file);
	Bitboard bishops = compute_bishop(own_side->bishops, own_side->all, opposing_side->all, clear_file);
	Bitboard queens = compute_queen(own_side->queens, own_side->all, opposing_side->all, clear_file);
	Bitboard king = compute_king(own_side->king, own_side->all, clear_file);
	return pawns | rooks | knights | bishops | queens | king;
}

Bitboard is_checked(Bitboard king, Bitboard attacked_squares) {
	return king & attacked_squares;
}

int calculate_stalemate(int player, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard en_passant_target) {
	Bitboard opponent_attacks = compute_attacking_squares(!player, opponent_pieces, player_pieces, en_passant_target);
	Bitboard in_check = is_checked(player_pieces->king, opponent_attacks);
	if (in_check) {
		return 0;
	}
	Bitboard valid_king_moves = compute_king(player_pieces->king, player_pieces->all, clear_file) & ~opponent_attacks;
	if (valid_king_moves) {
		return 0;
	}
	return 1;
}

Bitboard compute_king_attacks_diagonal(Bitboard king, char shift_amount, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]) {
    Bitboard attacks_on_king = 0x0;
    Bitboard pos_1 = king;
    Bitboard pos_2 = king;
	Bitboard pos_1_path = 0x0;
	Bitboard pos_2_path = 0x0;

    for (int i = 1; i <= RANK_LEN; i++) {
		if (pos_1_path & opposing_side) {
			continue;
		}
		if (pos_2_path & opposing_side) {
			continue;
		}
        if (shift_amount == 7) {
            pos_1 = pos_1 & clear_file[FILE_A];
            pos_2 = pos_2 & clear_file[FILE_H];
        } else if (shift_amount == 9 || shift_amount == 1) {
            pos_1 = pos_1 & clear_file[FILE_H];
            pos_2 = pos_2 & clear_file[FILE_A];
        }

        pos_1 = pos_1 << shift_amount & (~own_side); 
        pos_2 = pos_2 >> shift_amount & (~own_side);
		pos_1_path = pos_1_path | pos_1;
		pos_2_path = pos_2_path | pos_2;
    }
	if (pos_1_path & opposing_side) {
		attacks_on_king = attacks_on_king | pos_1_path;
	}
	if (pos_2_path & opposing_side) {
		attacks_on_king = attacks_on_king | pos_2_path;
	}
    return attacks_on_king;
}

// TODO: Ask question, merging stalemate and checked to prevent double computing, and letting return int determine game state
// (ex: 0 is game in proress, 1 is mate, 2 is stalemate) 
int is_mate(Chessboard *chessboard, int player) {
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
		Bitboard king_moves = compute_king(player_pieces->king, player_pieces->all, clear_file);
		if (king_moves & ~opponent_attacks) { // Check for stalemate.
			return 0;
		} else {
			return 2;
		}
	}

	Bitboard valid_king_moves = compute_king(player_pieces->king, opponent_pieces->all, clear_file);
	valid_king_moves = valid_king_moves & ~black_attacks;
	if (valid_king_moves) {
		return 0;
	}

	Bitboard horizontal_attacks = compute_king_attacks_diagonal(player_pieces->king, 1, player_pieces->all, opponent_pieces->all, clear_file);
	Bitboard vertical_attacks = compute_king_attacks_diagonal(player_pieces->king, 8, player_pieces->all, opponent_pieces->all, clear_file);
	Bitboard diagonal_1_attacks = compute_king_attacks_diagonal(player_pieces->king, 7, player_pieces->all, opponent_pieces->all, clear_file);
	Bitboard diagonal_2_attacks = compute_king_attacks_diagonal(player_pieces->king, 9, player_pieces->all, opponent_pieces->all, clear_file);
	Bitboard attacks_on_king = 	compute_knight(player_pieces->king, player_pieces->all, clear_file) & compute_knight(opponent_pieces->king, opponent_pieces->all, clear_file) |
									horizontal_attacks | vertical_attacks | diagonal_1_attacks | diagonal_2_attacks;

	// If move and not pin, not checkmate
	Bitboard possible_block_or_capture = player_attacks & attacks_on_king;
	if (!possible_block_or_capture) {
		return 1;
	}
	for (Bitboard a = 1ULL; a != 0; a = a<<1) {
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
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is divisible by 2 (only 1 bit is set to 1)
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
	}
	return 1;
}