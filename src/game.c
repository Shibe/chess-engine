#include "bitboard.h"

#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "input.h"
#include "game.h"
#include "gamestate.h"
#include <stdio.h>


void game_loop(Chessboard *chessboard) {
	while (1) {
		int active_player = chessboard->active_color;
		print_chessboard(chessboard);

		int game_state = get_gamestate(chessboard, active_player);
		if (game_state == GS_STALEMATE) {
			puts("Stalemate");
			return;
		} else if (game_state == GS_MATE) {
			if (active_player == WHITE){
				puts("Black mated white");
			} else {
				puts("White mated black");
			}
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
	int move_error = read_move(&start_file, &start_rank, &end_file, &end_rank);
	if (move_error) {
		return -1;
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

	Pieces own_side_copy = *own_side;
	Pieces opponent_side_copy = *opposing_side;
	move_piece(&own_side_copy, start, end);
	move_piece(&opponent_side_copy, end, 0x0ULL);
	update_pieces(&own_side_copy);
	update_pieces(&opponent_side_copy);
	Bitboard in_check = is_checked(own_side_copy.king, compute_attacking_squares(!active_player, &opponent_side_copy, &own_side_copy, en_passant_target));
	if (in_check) { // Moving causes king to be checked. Not allowed.
		return 0;
	}
	move_piece(own_side, start, end);
	move_piece(opposing_side, end, 0x0ULL);
	return 1;
}

Bitboard input_to_square(int rank, int file) {
	Bitboard location = 0x00000000000001ULL; // Bottom left
	int shift_amount = rank * 8 + file;
	location = location << shift_amount;
	return location;
}