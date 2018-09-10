#include <stdio.h>
#include "bitboard.h"

Bitboard get(Bitboard b, int square) {
    return (b & (1ULL << square));
}
    
void print_board(Bitboard b) {
    int i, j;
    for (i = RANK_8; i >= RANK_1; i--) {
        for (j = FILE_A; j <= FILE_H; j++) {
            printf ("%d ", get(b, j+8*i) ? 1 : 0);
        }
        printf ("\n");
    }
}