#include "bitboard.h"

Bitboard parse_rank(char rank[], int rank_count, Chessboard *board);
void parse(char* fen);
int get_shift_amount(char token);
void token_to_chessboard(char token, Bitboard location, Chessboard *chessboard);