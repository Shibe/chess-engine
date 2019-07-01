#include "chessboard.h"
#include "fen_parser.h"
#include "fixed_pieces.h"
#include "greatest.h"

TEST test_compute_king(void) {
	Chessboard *chessboard = create_chessboard();
	char *fen = "8/8/8/8/8/1P4p1/KP1K2pK/1P4p1 w - - 0 0";
	int error = parse_fen(chessboard, fen);
	if (error) {
		FAILm("Could not parse fen");
	}
	Bitboard result = compute_king(chessboard->white_pieces->king, chessboard->white_pieces->all, clear_file);
	Bitboard expected = 0xDD54DDULL;
	ASSERT_EQ_FMT(expected, result, "%llx");
	PASS();
}

TEST test_compute_knight(void) {
	Chessboard *chessboard = create_chessboard();
	char *fen = "8/8/8/1P4p1/8/N2N3N/8/1P4p1 w - - 0 0";
	int error = parse_fen(chessboard, fen);
	if (error) {
		FAIL();
	}
	Bitboard result = compute_knight(chessboard->white_pieces->knights, chessboard->white_pieces->all, clear_file);
	Bitboard expected = 0x5426002654ULL;
	ASSERT_EQ_FMT(expected, result, "%llx");
	PASS();
}

TEST test_compute_pawn_white(void) {
	Chessboard *chessboard = create_chessboard();
	char *fen = "rnbqkbnr/2p1pppp/8/3pP3/p7/1p6/PPPP1PPP/RNBQKBNR w KQkq d6 0 0";
	int error = parse_fen(chessboard, fen);
	if (error) {
		FAIL();
	}
	Bitboard result = compute_valid_pawn_moves(WHITE, chessboard->white_pieces->pawns, chessboard->white_pieces->all, chessboard->black_pieces->all, chessboard->en_passant_target, mask_rank, clear_file);
	Bitboard expected = 0x1000EEED0000ULL;
	ASSERT_EQ_FMT(expected, result, "%llx");
	PASS();
}

TEST test_compute_pawn_black(void) {
	Chessboard *chessboard = create_chessboard();
	char *fen = "rnbqkbnr/ppp1pppp/6P1/7P/3pP3/8/PPPP1P2/RNBQKBNR b KQkq e3 0 0";
	int error = parse_fen(chessboard, fen);
	if (error) {
		FAIL();
	}
	Bitboard result = compute_valid_pawn_moves(BLACK, chessboard->black_pieces->pawns, chessboard->black_pieces->all, chessboard->white_pieces->all, chessboard->en_passant_target, mask_rank, clear_file);
	Bitboard expected = 0xB77700080000ULL;
	ASSERT_EQ_FMT(expected, result, "%llx");
	PASS();
}

SUITE(fixed_pieces_suite) {
	RUN_TEST(test_compute_king);
	RUN_TEST(test_compute_knight);
	RUN_TEST(test_compute_pawn_white);
	RUN_TEST(test_compute_pawn_black);
}
