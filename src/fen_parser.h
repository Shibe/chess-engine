#include "bitboard.h"

int parse_rank_token(char rank[], int rank_count, Chessboard *board);
int parse(char* fen);
void token_to_chessboard(char token, Bitboard location, Chessboard *chessboard);