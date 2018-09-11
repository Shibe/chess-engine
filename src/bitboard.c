#include <stdio.h>
#include "bitboard.h"

Bitboard mask_rank[RANK_LEN] = { 0xFFULL, 0xFF00ULL, 0x0FF0000ULL, 0x0FF000000ULL,
														0x0FF00000000ULL, 0x0FF0000000000ULL, 
														0x0FF000000000000ULL, 0x0FF00000000000000ULL };

Bitboard clear_rank[RANK_LEN] = { 0xFFFFFFFFFFFFFF00ULL, 0xFFFFFFFFFFFF00FFULL, 
														0xFFFFFFFFFF00FFFFULL, 0xFFFFFFFF00FFFFFFULL,
														0xFFFFFF00FFFFFFFFULL, 0xFFFF00FFFFFFFFFFULL, 
														0xFF00FFFFFFFFFFFFULL, 0x00FFFFFFFFFFFFFFULL };                          

Bitboard clear_file[FILE_LEN] = { 0xFEFEFEFEFEFEFEFEULL, 0xFDFDFDFDFDFDFDFDULL, 0xFBFBFBFBFBFBFBFBULL, 0x0,
																		0x0, 0x0, 0x0, 0x7F7F7F7F7F7F7F7FULL};

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