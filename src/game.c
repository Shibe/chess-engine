#include "bitboard.h";
#include "chessboard.h";
#include "fixed_pieces.h";
#include "sliding_pieces.h";
#include "game.h";

void game_loop(Chessboard *chessboard) {

}

void turn(Pieces *own_side, Pieces *opposing_side) {
}

void compute_attacked_squares(Pieces *own_side, Pieces *opposing_side) {
	Bitboard pawns = 0x0; //TODO: Add pawn
	Bitboard rooks = compute_rook(opposing_side->rooks, opposing_side->all, own_side->all, clear_file);
	Bitboard knights = compute_knight(opposing_side->knights, own_side->all, clear_file);
	Bitboard bishops = compute_bishop(opposing_side->bishops, opposing_side->all, own_side->all, clear_file);
	Bitboard queens = compute_queen(opposing_side->queens, opposing_side->all, own_side, clear_file);
	Bitboard king = compute_king(opposing_side->king, opposing_side->all, clear_file);
	return pawns | rooks | knights | bishops | queens | king;
}

int is_checked(Bitboard king, Bitboard attacked_squares) {
	return king & attacked_squares;
}