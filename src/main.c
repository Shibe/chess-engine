#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum files { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_LEN };
enum ranks { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_LEN };

typedef uint64_t Bitboard;

typedef struct {
    Bitboard white_pawns;
    Bitboard white_rooks;
    Bitboard white_knights;
    Bitboard white_bishops;
    Bitboard white_queens;
    Bitboard white_king;

    Bitboard black_pawns;
    Bitboard black_rooks;
    Bitboard black_knights;
    Bitboard black_bishops;
    Bitboard black_queens;
    Bitboard black_king;

    Bitboard white_pieces;
    Bitboard black_pieces;
    Bitboard all_pieces;
} Chessboard;

void update_board(Chessboard *chessboard) {
    chessboard->white_pieces = chessboard->white_pawns | chessboard->white_rooks | 
                                chessboard->white_knights | chessboard->white_bishops |
                                chessboard->white_queens | chessboard->white_king;
    chessboard->black_pieces = chessboard->black_pawns | chessboard->black_rooks | 
                                    chessboard->black_knights | chessboard->black_bishops |
                                    chessboard->black_queens | chessboard->black_king;
    chessboard->all_pieces = chessboard->white_pieces | chessboard->black_pieces;
}

Chessboard *initialise_board() {
    Chessboard *chessboard;
    if ((chessboard = malloc(sizeof *chessboard)) != NULL) {
        chessboard->white_pawns = 0xFF00ULL;
        chessboard->white_rooks = 0x81ULL;
        chessboard->white_knights = 0x42ULL;
        chessboard->white_bishops = 0x24ULL;
        chessboard->white_queens = 0x8ULL;
        chessboard->white_king = 0x10ULL;
        chessboard->black_pawns = 0xFF000000000000ULL;
        chessboard->black_rooks = 0x8100000000000000ULL;
        chessboard->black_knights = 0x4200000000000000ULL;
        chessboard->black_bishops = 0x2400000000000000ULL;
        chessboard->black_queens = 0x800000000000000ULL;
        chessboard->black_king = 0x1000000000000000ULL;
        update_board(chessboard);
    }
    return chessboard;
}

/*
    Basic idea: 

    A | B | C | D | E | F | G | H
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 1 | 2 | 3 | 0 | 0 | 0
    0 | 0 | 8 | K | 4 | 0 | 0 | 0
    0 | 0 | 7 | 6 | 5 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

    Bitboard of the king: 
    MSB 00000000 00000000 00000000 00001000 00000000 00000000 00000000 00000000 LSB

    If we want to move the king to pos 8: 
    -  Shift the bitboard of the king 1 to the right
    
    Result:
    MSB 00000000 00000000 00000000 00000100 00000000 00000000 00000000 00000000 LSB

    or:

    A | B | C | D | E | F | G | H
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 1 | 2 | 3 | 0 | 0 | 0 | 0
    0 | 8 | K | 4 | 0 | 0 | 0 | 0
    0 | 7 | 6 | 5 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

    We could take this resulting bitboard and & it against the pieces of your own side to check if 
    it could actually move there. (For now ignoring checks and assuming you can take any piece etc.)

    What if the king is on the A or H file?

    A | B | C | D | E | F | G | H
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    2 | 3 | 0 | 0 | 0 | 0 | 0 | 0
    K | 4 | 0 | 0 | 0 | 0 | 0 | 0
    6 | 5 | 0 | 0 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

    Bitboard of the king: 
    MSB 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00000000 LSB
    
    Obviously moving to spots 1, 7 and 8 is illegal.

    For example moving to spot 8 (shifting the bitboard 1 to the right) will result in:

    MSB 00000000 00000000 00000000 00000000 10000000 00000000 00000000 00000000 LSB

    A | B | C | D | E | F | G | H
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 1 | 2
    0 | 0 | 0 | 0 | 0 | 0 | 8 | K 
    0 | 0 | 0 | 0 | 0 | 0 | 7 | 6
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

    Which is obviously an illegal move.

    This is solved by removing the king from the A file when checking for moves 1, 7 and 8.

    -------------------------------------------------------------------------------

    Clearfile Bitboard for the A file:

    MSB 11111110 11111110 11111110 11111110 11111110 11111110 11111110 11111110 LSB
    
    &

    Position of the king: 

    MSB 00000000 00000000 00000000 00000001 00000000 00000000 00000000 00000000 LSB

    = 

    MSB 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 LSB

    -------------------------------------------------------------------------------

    Shifting the result 1 to the right wtill result in:

    MSB 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 LSB

    This means the move is now allowed.

    The same should be done using the H file when checking moves 3, 4 and 5.

    If a king is located on the 1rst of 8th rank, there should be no problem, unless its
    also located on the A or H file.

*/
Bitboard compute_king(Bitboard king_loc, Bitboard own_side, Bitboard clear_file[8]) {
    Bitboard king_removed_file_a = king_loc & clear_file[FILE_A]; // file A
    Bitboard king_removed_file_h = king_loc & clear_file[FILE_H]; // file H

    Bitboard pos_1 = king_removed_file_h << 7;
    Bitboard pos_2 = king_loc << 8;
    Bitboard pos_3 = king_removed_file_h << 9;
    Bitboard pos_4 = king_removed_file_h << 1;
    Bitboard pos_5 = king_removed_file_a >> 7;
    Bitboard pos_6 = king_loc >> 8;
    Bitboard pos_7 = king_removed_file_a >> 9;
    Bitboard pos_8 = king_removed_file_a >> 1;

    Bitboard king_moves = pos_1 | pos_2 | pos_3 |
                            pos_4 | pos_5 | pos_6 |
                            pos_7 | pos_8;

    Bitboard open_moves_own_side = ~own_side;
    Bitboard valid_king_moves = king_moves & open_moves_own_side;

    return valid_king_moves;
}

int main() {
    Chessboard *chessboard = initialise_board();
    
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
    
    /*
        Only created for file A and H, cba right now.
    */
    Bitboard clear_file[FILE_LEN] = { 0xFEFEFEFEFEFEFEFEULL, 0x0, 0x0, 0x0,
                                        0x0, 0x0, 0x0, 0x7F7F7F7F7F7F7F7FULL};
    

    puts("Checking moves for white king from starting position");

    Bitboard open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %lx \n", (chessboard->white_king));
    printf("White pieces : %lx \n", (chessboard->white_pieces));
    printf("Black pieces : %lx \n", (chessboard->black_pieces));
    printf("All pieces : %lx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %lx \n", (open_moves));

    puts("\nChecking moves for white king on D5.");

    // Position of the king.
    Bitboard D5 = 0x800000000ULL;

    chessboard->white_king = D5;
    update_board(chessboard);

    open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %lx \n", (chessboard->white_king));
    printf("White pieces : %lx \n", (chessboard->white_pieces));
    printf("Black pieces : %lx \n", (chessboard->black_pieces));
    printf("All pieces : %lx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %lx \n", (open_moves));

    puts("\nChecking moves for white king on A5.");

    // Position of the king.
    Bitboard A5 = 0x100000000ULL;

    chessboard->white_king = A5;
    update_board(chessboard);

    open_moves = compute_king(chessboard->white_king, chessboard->white_pieces, clear_file);

    printf("White king : %lx \n", (chessboard->white_king));
    printf("White pieces : %lx \n", (chessboard->white_pieces));
    printf("Black pieces : %lx \n", (chessboard->black_pieces));
    printf("All pieces : %lx \n", (chessboard->all_pieces));
    printf("Open moves for the white king : %lx \n", (open_moves));

    return 0;
}