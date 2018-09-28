#include "chessboard.h"
#include "fen_parser.h"
#include "gamestate.h"
#include "greatest.h"

TEST test_is_mate(void) {
	SKIP();
}

TEST test_is_stalemate(void) {
	SKIP();
}

TEST test_get_gamestate(void) {
	SKIP();
}

TEST test_attempt_move(void) {
	SKIP();
}

TEST test_attack_square(void) {
	SKIP();
}

TEST test_is_checked(void) {
	SKIP();
}

TEST test_compute_attacking_squares(void) {
	SKIP();
}

TEST test_move_piece(void) {
	SKIP();
}

SUITE(gamestate_suite) {
	RUN_TEST(test_is_mate);
    RUN_TEST(test_is_stalemate);
	RUN_TEST(test_get_gamestate);
	RUN_TEST(test_attempt_move);
	RUN_TEST(test_attack_square);
	RUN_TEST(test_is_checked);
	RUN_TEST(test_compute_attacking_squares);
	RUN_TEST(test_move_piece);
}
