#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "input.h"
#include "game.h"
#include <stdio.h>


void game_loop(Chessboard *chessboard) {
	while (1) {
		print_chessboard(chessboard);
		int active_player = chessboard->active_color;
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
		 	{
				if (active_player == WHITE) {
					end = *en_passant_target >> 8;
				} else {
					end = *en_passant_target << 8;
				}
			 }
			 move_piece(opposing_side, end, 0x0ULL);
			 return 1;
		} else {
			*en_passant_target = 0x0ULL;
		}
		move_piece(own_side, start, end);
		move_piece(opposing_side, end, 0x0ULL);
	} else {
		move_piece(own_side, start, end);
		move_piece(opposing_side, end, 0x0ULL);
		*en_passant_target = 0x0ULL;
	}
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

Bitboard compute_attacked_squares(int opposing_player, Pieces *own_side, Pieces *opposing_side, Bitboard en_passant_target) {
	Bitboard pawns = compute_pawn(opposing_player, opposing_side->pawns, opposing_side->all, en_passant_target, own_side->all, mask_rank, clear_file); //TODO: Not every valid move is an attacked square, should be split up.
	Bitboard rooks = compute_rook(opposing_side->rooks, opposing_side->all, own_side->all, clear_file);
	Bitboard knights = compute_knight(opposing_side->knights, opposing_side->all, clear_file);
	Bitboard bishops = compute_bishop(opposing_side->bishops, opposing_side->all, own_side->all, clear_file);
	Bitboard queens = compute_queen(opposing_side->queens, opposing_side->all, own_side->all, clear_file);
	Bitboard king = compute_king(opposing_side->king, opposing_side->all, clear_file);
	return pawns | rooks | knights | bishops | queens | king;
}

int is_checked(Bitboard king, Bitboard attacked_squares) {
	return king & attacked_squares;
}
