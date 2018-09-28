#include "chessboard.h"
#include "fen_parser.h"
#include "sliding_pieces.h"
#include "greatest.h"

TEST test_compute_queen(void) {
	SKIP();
}

TEST test_compute_rook(void) {
	SKIP();
}

TEST test_compute_bishop(void) {
	SKIP();
}

SUITE(sliding_pieces_suite) {
	RUN_TEST(test_compute_queen);
    RUN_TEST(test_compute_rook);
	RUN_TEST(test_compute_bishop);
}
