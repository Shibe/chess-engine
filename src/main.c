#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "minunit.h"
#include "game.h"
#include "fen_parser.h"

int tests_run = 0;

static char *test_king_attacks() {
    Bitboard king_loc = 0x8000000000ULL;
    Bitboard white_pieces = 0xC000000000ULL;

    Bitboard outcome_attacks = compute_king(king_loc, white_pieces, clear_file);
    Bitboard expected_attacks = 0xC000C0000000ULL;
		char *message = malloc(128 * sizeof(char)); 
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
		free(message);
    return 0;
}

static char *test_knight_attacks() {
    Bitboard knight_loc = 0x9000000000ULL;
    Bitboard white_pieces = 0x9020400000ULL;

    Bitboard outcome_attacks = compute_knight(knight_loc, white_pieces, clear_file);
    Bitboard expected_attacks = 0x68640044280000ULL;
    char *message = malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
		free(message);
    return 0;
}

static char *test_queen_attacks() {
    Bitboard queen_loc = 0x10000000ULL;
    Bitboard white_pieces = 0x10005400ULL;
		Bitboard black_pieces = 0xFF000000000000ULL;

    Bitboard outcome_attacks = compute_queen(queen_loc, white_pieces, black_pieces, clear_file);
    Bitboard expected_attacks = 0x925438EF380000ULL;
    char *message = malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
		free(message);
    return 0;
}

static char *test_rook_attacks() {
    Bitboard rook_loc = 0x10000000ULL;
    Bitboard white_pieces = 0x10005400ULL;
		Bitboard black_pieces = 0xFF000000000000ULL;

    Bitboard outcome_attacks = compute_rook(rook_loc, white_pieces, black_pieces, clear_file);
    Bitboard expected_attacks = 0x101010EF100000ULL;
    char *message = malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
		free(message);
    return 0;
}

static char *test_bishop_attacks() {
    Bitboard bishop_loc = 0x10000000ULL;
    Bitboard white_pieces = 0x10005400ULL;
		Bitboard black_pieces = 0xFF000000000000ULL;

    Bitboard outcome_attacks = compute_bishop(bishop_loc, white_pieces, black_pieces, clear_file);
    Bitboard expected_attacks = 0x82442800280000ULL;
    char *message = malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
		free(message);
    return 0;
}

static char *test_black_pawn_attacks_from_start() {
    Bitboard pawn_loc = 0x80000000000000ULL;
    Bitboard white_pieces = 0x1014140FF000000ULL;
    Bitboard black_pieces = 0xFEFE000000000000ULL;
    Bitboard en_passant_target = 0x0ULL;

    Bitboard outcome_attacks = compute_pawn(BLACK, pawn_loc, black_pieces, white_pieces, en_passant_target, mask_rank, clear_file);
    Bitboard expected_attacks = 0xC08000000000ULL;
    char *message = malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
		free(message);
    return 0;
}

static char *test_white_pawn_attacks(){
    Bitboard pawn_loc = 0x200000ULL;
    Bitboard white_pieces = 0x200000ULL;
    Bitboard black_pieces = 0x2010000000ULL;
    Bitboard en_passant_target = 0x0ULL;

    Bitboard outcome_attacks = compute_pawn(WHITE, pawn_loc, white_pieces, black_pieces, en_passant_target, mask_rank, clear_file);
    Bitboard expected_attacks = 0x30000000ULL;
    char *message = malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
    free(message);
    return 0;
}

static char* test_en_passant_take() {
    Chessboard *chessboard = create_chessboard();
    char *fen = "rnbqkbnr/1pp1pppp/p7/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3";
    Bitboard start = 0x1000000000ULL;
    Bitboard end = 0x80000000000ULL;
    char *message = malloc(128 * sizeof(char));
    int error = parse(chessboard, fen);
    if (error) {
        sprintf(message, "Could not parse fen: %s.", fen);
        mu_assert(message, 0);
    }
    int success = turn(start, end, chessboard->active_color, chessboard->white_pieces, chessboard->black_pieces, &chessboard->en_passant_target);
    if (!success) {
        sprintf(message, "Could not move piece. En passant: %lx, start: %lx, end: %lx.", chessboard->en_passant_target, start, end);
        mu_assert(message, 0);        
    }
    update_chessboard(chessboard);

    int white_moved = (chessboard->white_pieces->pawns & end) == end;
    int black_removed = (chessboard->black_pieces->pawns & (end << 8)) == 0x0ULL;
    if (!white_moved) {
        sprintf(message, "White pawn moves incorrectly; outcome: %lx",  chessboard->white_pieces->pawns & end);
        mu_assert(message, 0);
    } else if (!black_removed) {
        sprintf(message, "Black pawn removed incorrectly; outcome: %lx",  chessboard->black_pieces->pawns & (end << 8));
        mu_assert(message, 0);
    }
    free(message);
    return 0;
}

static char *all_tests() {
    mu_run_test(test_king_attacks);
    mu_run_test(test_knight_attacks);
    mu_run_test(test_queen_attacks);
    mu_run_test(test_rook_attacks);
    mu_run_test(test_bishop_attacks);
    mu_run_test(test_black_pawn_attacks_from_start);
    mu_run_test(test_white_pawn_attacks);
    mu_run_test(test_en_passant_take);
    return 0;
}

int main(int argc, char *argv[]) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    if (result) {
        return 1;
    }

    Chessboard *chessboard = create_chessboard();
    if (chessboard == NULL) {
        return 1;
    }
    if (argc > 1) {
        char *fen = argv[1];
        parse(chessboard, fen);
    } else {
        initialise_chessboard(chessboard);
    }

    // game_loop(chessboard);  
    char *fen = malloc(128 * sizeof(char));
    reverse_parse(chessboard, fen);

    return 0;
}
