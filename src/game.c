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
		int game_state = is_mate(chessboard, active_player);
		if (game_state == 2) {
			puts("Stalemate");
			return;
		} else if (game_state == 1) {
			puts("Checkmate");
			return;
		}

		print_chessboard(chessboard);
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
	Bitboard in_check = is_checked(own_side->king, compute_attacking_squares(!active_player, opposing_side, own_side, en_passant_target));
	if (in_check) { // Moving causes king to be checked. Not allowed.
		move_piece(own_side, end, start);
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

int is_stalemate(Chessboard *chessboard, int player) {	
	Bitboard white_attacks = compute_attacking_squares(WHITE, chessboard->white_pieces, chessboard->black_pieces, chessboard->en_passant_target);
	Bitboard black_attacks = compute_attacking_squares(BLACK, chessboard->black_pieces, chessboard->white_pieces, chessboard->en_passant_target);

	if (player == WHITE){
		int in_check = is_checked(chessboard->white_pieces->king, black_attacks);
		return !in_check && ~white_attacks;
	} else {
		int in_check = is_checked(chessboard->black_pieces->king, white_attacks);
		return !in_check && ~black_attacks;
	}
}

int calculate_stalemate(int player, Pieces *player_pieces, Pieces *opponent_pieces, Bitboard en_passant_target) {
	Bitboard opponent_attacks = compute_attacking_squares(player, player_pieces, opponent_pieces, en_passant_target);
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
		if (calculate_stalemate(player, player_pieces, opponent_pieces, chessboard->en_passant_target)) { // Check for stalemate.
			return 2;
		} else {
			return 0;
		}
	}

	Bitboard valid_king_moves = compute_king(player_pieces->king, opponent_pieces->all, clear_file);
	valid_king_moves = valid_king_moves & ~black_attacks;
	if (valid_king_moves) {
		return 0;
	}

	Bitboard attacks_on_king = compute_queen(player_pieces->king, player_pieces->all, opponent_pieces->all, clear_file) & compute_queen(opponent_pieces->queens, opponent_pieces->all, player_pieces->all, clear_file);
								compute_bishop(player_pieces->king, player_pieces->all, opponent_pieces->all, clear_file) & compute_bishop(opponent_pieces->bishops, opponent_pieces->all, player_pieces->all, clear_file) |
								compute_rook(player_pieces->king, player_pieces->all, opponent_pieces->all, clear_file) & compute_rook(opponent_pieces->king, opponent_pieces->all, player_pieces->all, clear_file) |
								compute_knight(player_pieces->king, player_pieces->all, clear_file) & compute_knight(opponent_pieces->king, opponent_pieces->all, clear_file) |
								compute_pawn(player, player_pieces->king, player_pieces->all, opponent_pieces->all, chessboard->en_passant_target, mask_rank, clear_file) &
									compute_pawn(!player, opponent_pieces->pawns, opponent_pieces->all, player_pieces->all, chessboard->en_passant_target, mask_rank, clear_file);

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
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->pawns = copy;
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
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->queens = copy;
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
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->rooks = copy;
				if (!checked) {
					return 0;	
				}
			}
		}
		if (player_pieces->bishops & a) {
			Bitboard intercepts = compute_bishop(player_pieces->bishops & a, player_pieces->all, opponent_pieces->all, clear_file) & possible_block_or_capture;
			if (intercepts && (!(intercepts&(intercepts-1)))) { // If the amount of intecepts is divisible by 2 (only 1 bit is set to 1)
				Bitboard copy = player_pieces->bishops;
				player_pieces->bishops = (player_pieces->bishops & ~a) | intercepts;
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->bishops = copy;
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
				Bitboard checked = is_checked(player_pieces->king, compute_attacking_squares(!player, opponent_pieces, player_pieces, chessboard->en_passant_target));
				player_pieces->knights = copy;
				if (!checked) {
					return 0;	
				}
			}
		}
	}
	return 1;
}