#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"
#include "minunit.h"

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

static char *all_tests() {
    mu_run_test(test_king_attacks);
    mu_run_test(test_knight_attacks);
		mu_run_test(test_queen_attacks);
		mu_run_test(test_rook_attacks);
		mu_run_test(test_bishop_attacks);
    return 0;
}

int main() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
