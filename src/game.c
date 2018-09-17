#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "input.h"
#include "game.h"
#include <stdio.h>


void game_loop(Chessboard *chessboard) {
	while (1) {
		int success = 0;
		int active_player = chessboard->active_color;
		print_chessboard(chessboard);
		if (chessboard->active_color == WHITE) {
			printf("%s", "White player enter your move.\n");
			success = turn(active_player, chessboard->white_pieces, chessboard->black_pieces);
		} else {
			printf("%s", "Black player enter your move.\n");
			success = turn(active_player, chessboard->black_pieces, chessboard->white_pieces);
		}
		if (success) {
			update_chessboard(chessboard);
			chessboard->active_color = !active_player;
		}
	}
}

int turn(int active_player, Pieces *own_side, Pieces *opposing_side) {
	Bitboard attacked_squares = compute_attacked_squares(!active_player, own_side, opposing_side);
	int start_file, start_rank, end_file, end_rank;
	int correct_move = read_move(&start_file, &start_rank, &end_file, &end_rank);
	if (!correct_move) {
		return 0;
	}

	Bitboard start = input_to_square(start_rank, start_file);
	Bitboard end = input_to_square(end_rank ,end_file);
	Bitboard is_valid_move = 0x0ULL;

	if (!(own_side->all & start)) {
		return 0;
	}
	
	if (start & own_side->pawns) {
		is_valid_move = end & compute_pawn(active_player, own_side->pawns & start, own_side->all, opposing_side->all, mask_rank, clear_file);
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

	move_piece(own_side, start, end);
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

Bitboard compute_attacked_squares(int opposing_player, Pieces *own_side, Pieces *opposing_side) {
	Bitboard pawns = compute_pawn(opposing_player, opposing_side->pawns, opposing_side->all, own_side->all, mask_rank, clear_file); //TODO: Not every valid move is an attacked square, should be split up.
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
