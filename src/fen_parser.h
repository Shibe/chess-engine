#include "bitboard.h"

Bitboard parse_rank(char rank[], int rank_count, Bitboard board);
void parse(char* fen);
int get_shift_amount(char token);