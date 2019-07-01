#include "chessboard.h"
#include "fen_parser.h"
#include "fen_serializer.h"
#include "greatest.h"

TEST test_create_fen(void) {
    Chessboard *chessboard = create_chessboard();
    char *fen = "rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";
    int error = parse_fen(chessboard, fen);
    if (error) {
        FAILm("Failed to parse fen.");
    }
    char *serialized;
    serialize_fen(chessboard, &serialized);
    ASSERT_EQ_FMT(*fen, *serialized, "%s");
    PASS();
}

SUITE(fen_serializer_suite) {
    RUN_TEST(test_create_fen);
}