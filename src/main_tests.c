#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "greatest.h"

SUITE_EXTERN(fixed_pieces_suite);
SUITE_EXTERN(sliding_pieces_suite);
SUITE_EXTERN(gamestate_suite);
SUITE_EXTERN(fen_serializer_suite);

GREATEST_MAIN_DEFS();

int main(int argc, char *argv[]) {
	GREATEST_MAIN_BEGIN();
	RUN_SUITE(fixed_pieces_suite);
	RUN_SUITE(sliding_pieces_suite);
	RUN_SUITE(gamestate_suite);
	RUN_SUITE(fen_serializer_suite);
	GREATEST_MAIN_END();
}