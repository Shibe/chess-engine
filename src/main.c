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
    char *message = (char*)malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
    return 0;
}

static char *test_knight_attacks() {
    Bitboard knight_loc = 0x9000000000ULL;
    Bitboard white_pieces = 0x9020400000ULL;

    Bitboard outcome_attacks = compute_knight(knight_loc, white_pieces, clear_file);
    Bitboard expected_attacks = 0x68640044280000ULL;
    char *message = (char*)malloc(128 * sizeof(char));
    sprintf(message, "outcome: %lx != expected: %lx", outcome_attacks, expected_attacks);
    mu_assert(message, outcome_attacks == expected_attacks);
    return 0;
}

static char *all_tests() {
    mu_run_test(test_king_attacks);
    mu_run_test(test_knight_attacks);
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
