#include "bitboard.h"

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

Bitboard compute_king(Bitboard king_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]) {
    Bitboard king_removed_file_a = king_loc & clear_file[FILE_A];
    Bitboard king_removed_file_h = king_loc & clear_file[FILE_H];

    Bitboard pos_1 = king_removed_file_a << 7;
    Bitboard pos_2 = king_loc << 8;
    Bitboard pos_3 = king_removed_file_h << 9;
    Bitboard pos_4 = king_removed_file_h << 1;
    Bitboard pos_5 = king_removed_file_h >> 7;
    Bitboard pos_6 = king_loc >> 8;
    Bitboard pos_7 = king_removed_file_a >> 9;
    Bitboard pos_8 = king_removed_file_a >> 1;

    Bitboard king_attacks = pos_1 | pos_2 | pos_3 |
                            pos_4 | pos_5 | pos_6 |
                            pos_7 | pos_8;

    Bitboard open_attacks_own_side = ~own_side;
    Bitboard valid_king_attacks = king_attacks & open_attacks_own_side;

    return valid_king_attacks;
}

Bitboard compute_knight(Bitboard knight_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]) {
		return knight_loc;
}