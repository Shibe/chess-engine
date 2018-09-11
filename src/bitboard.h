#include <stdint.h>
#ifndef BITBOARD_FILE
#define BITBOARD_FILE

enum files { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_LEN };
enum ranks { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_LEN };

typedef uint64_t Bitboard;

Bitboard get(Bitboard b, int square);

void print_board(Bitboard b);

#endif