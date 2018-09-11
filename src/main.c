#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "bitboard.h"
#include "chessboard.h"
#include "fixed_pieces.h"
#include "sliding_pieces.h"

int main() {
    Chessboard *chessboard = initialise_chessboard();
    /*
        Testing purposes only. I know this is a mess.
        Don't really need mask_rank and clear_rank right now, but these are useful later.
    */
		Bitboard mask_rank[RANK_LEN] = { 0xFFULL, 0xFF00ULL, 0x0FF0000ULL, 0x0FF000000ULL,
																0x0FF00000000ULL, 0x0FF0000000000ULL, 
																0x0FF000000000000ULL, 0x0FF00000000000000ULL };

		Bitboard clear_rank[RANK_LEN] = { 0xFFFFFFFFFFFFFF00ULL, 0xFFFFFFFFFFFF00FFULL, 
																0xFFFFFFFFFF00FFFFULL, 0xFFFFFFFF00FFFFFFULL,
																0xFFFFFF00FFFFFFFFULL, 0xFFFF00FFFFFFFFFFULL, 
																0xFF00FFFFFFFFFFFFULL, 0x00FFFFFFFFFFFFFFULL };                          

		Bitboard clear_file[FILE_LEN] = { 0xFEFEFEFEFEFEFEFEULL, 0xFDFDFDFDFDFDFDFDULL, 0xFBFBFBFBFBFBFBFBULL, 0x0,
																				0x0, 0x0, 0x0, 0x7F7F7F7F7F7F7F7FULL};
    
    print_board(clear_file[FILE_A]);
    puts("Checking moves for white king from starting position");

    Bitboard open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %llx \n", (open_moves));
    print_board(open_moves);
    puts("\nChecking moves for white king on D5.");

    // Position of the king.
    Bitboard D5 = 0x800000000ULL;

    chessboard->white_king = D5;
    update_chessboard(chessboard);

    open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %llx \n", (open_moves));
    print_board(open_moves);
    puts("\nChecking moves for white king on A5.");

    // Position of the king.
    Bitboard A5 = 0x100000000ULL;
    chessboard->white_king = A5;
    update_chessboard(chessboard);

    open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %llx \n", (open_moves));
    print_board(open_moves);

    Bitboard H5 = 0x8000000000ULL;
    chessboard->white_king = H5;
    update_chessboard(chessboard);

    open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %llx \n", (open_moves));
    print_board(open_moves);

    puts("\nChecking moves for queen on D5.");

    chessboard->white_queens = D5;
    update_chessboard(chessboard);

    open_moves = compute_queen(chessboard->white_queens, chessboard->white_pieces, chessboard->black_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the queen : %llx \n", (open_moves));
    print_board(open_moves);

    free(chessboard);
    chessboard = initialise_chessboard();
    puts("\nChecking moves for both rooks on C5 and F6.");


    chessboard->white_rooks = 0x000400000000;
    update_chessboard(chessboard);

    open_moves = compute_rook(chessboard->white_rooks, chessboard->white_pieces, chessboard->black_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the rooks : %llx \n", (open_moves));

    free(chessboard);
    chessboard = initialise_chessboard();
    puts("\nChecking moves for both bishops on C5 and F6.");

    chessboard->white_bishops = 0x8000000000;
    update_chessboard(chessboard);

    open_moves = compute_bishop(chessboard->white_bishops, chessboard->white_pieces, chessboard->black_pieces, clear_file);

    printf("White king : %llx \n", (chessboard->white_king));
    printf("White pieces : %llx \n", (chessboard->white_pieces));
    printf("Black pieces : %llx \n", (chessboard->black_pieces));
    printf("All pieces : %llx \n", (chessboard->all_pieces));
    printf("Open moves for the bishops : %llx \n", (open_moves));

    return 0;
}
