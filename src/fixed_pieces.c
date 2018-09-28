#include "bitboard.h"
#include "chessboard.h"

#include <stdio.h>
#include <ctype.h>

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

/*
	Basic idea for knights:

    A | B | C | D | E | F | G | H
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 
    0 | 0 | 0 | 2 | 0 | 3 | 0 | 0
    0 | 0 | 1 | 0 | 0 | 0 | 4 | 0
    0 | 0 | 0 | 0 | K | 0 | 0 | 0
    0 | 0 | 8 | 0 | 0 | 0 | 5 | 0 
    0 | 0 | 0 | 7 | 0 | 6 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

	Given knight at K, he can jump to the different positions. Similar to the king
	we must solve the wrap around problem. The differnece is the wrap around problem can
	occur in not only the A and H file, but also the B and G files. As such wrap around for
	those position numbers must also clear the B and G files.
*/

Bitboard compute_knight(Bitboard knight_loc, Bitboard own_side, Bitboard clear_file[FILE_LEN]) {
	Bitboard pos_1 = knight_loc << 6 & clear_file[FILE_H] & clear_file[FILE_G];
	Bitboard pos_2 = knight_loc << 15 & clear_file[FILE_H];
	Bitboard pos_3 = knight_loc << 17 & clear_file[FILE_A];
	Bitboard pos_4 = knight_loc << 10 & clear_file[FILE_A] & clear_file[FILE_B];
	Bitboard pos_5 = knight_loc >> 6 & clear_file[FILE_A] & clear_file[FILE_B];
	Bitboard pos_6 = knight_loc >> 15 & clear_file[FILE_A];
	Bitboard pos_7 = knight_loc >> 17 & clear_file[FILE_H];
	Bitboard pos_8 = knight_loc >> 10 & clear_file[FILE_H] & clear_file[FILE_G];

	Bitboard knight_attacks = pos_1 | pos_2 | pos_3 |
							pos_4 | pos_5 | pos_6 |
							pos_7 | pos_8;

	Bitboard open_attacks_own_side = ~own_side;
	Bitboard valid_knight_attacks = knight_attacks & open_attacks_own_side;

	return valid_knight_attacks;
}

/*
       A | B | C | D | E | F | G | H
    8  0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 
    7  0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    6  0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    5  0 | 0 | 0 | 0 | 0 | 0 | 0 | 0
    4  0 | 0 | 2 | 0 | 0 | 0 | 0 | 0 
    3  0 | 3 | 1 | 4 | 0 | 0 | 0 | 0
    2  0 | 0 | P | 0 | 0 | 0 | 0 | 0
    1  0 | 0 | 0 | 0 | 0 | 0 | 0 | 0

    The pawn is able to make four different moves, two out of which consists of attacking an enemy piece. 
    
    Pawns can only move vertically when moving and diagonally when attacking. 
    
    Given white pawn P, the pawn can move to 1 if there are neither enemy or friendly pieces on tile 1. Whenever the pawn is in
    the starting position (RANK_2 for white and RANK_7 for black), a pawn can move 2 steps in the vertical foward direction, 
    whenever there is no friendly or enemy piece in either of the two tiles in front of the pawn. This is checked by the 
    bitwise & between the pawn location and a mask for the starting rank for the pawn. 
    
    Mask of RANK_2 looks like 0xFF00 and the pawn position 0x400.
    
    Pawn P can move to 3 or 4 whenever an enemy piece is present at the corresponding tile.
    To avoid the wrap around problem for diagonal movements, clearing File H and File A are done accordingly.
*/

Bitboard compute_pawn(int active_player, Bitboard pawn_loc, Bitboard own_side, Bitboard opposing_side, Bitboard en_passant_target, Bitboard mask_rank[RANK_LEN], Bitboard clear_file[FILE_LEN]) {
    Bitboard pos_1 = 0x0ULL, pos_2 = 0x0ULL, pos_3 = 0x0ULL, pos_4 = 0x0ULL, pawn_on_start_position = 0x0ULL;
    
    if (active_player == BLACK) { 
        pos_1 = pawn_loc >> 8;

        pawn_on_start_position = pawn_loc & mask_rank[RANK_7];
        if (pawn_on_start_position && pos_1) {
            pos_2 = pawn_loc >> 16;
        }
        pos_3 = pawn_loc >> 7 & clear_file[FILE_A];
        pos_4 = pawn_loc >> 9 & clear_file[FILE_H];

    } else {
        pos_1 = pawn_loc << 8;
        pawn_on_start_position = pawn_loc & mask_rank[RANK_2];

        if (pawn_on_start_position && pos_1) {
            pos_2 = pawn_loc << 16;
        }

        pos_3 = pawn_loc << 7 & clear_file[FILE_H];
        pos_4 = pawn_loc << 9 & clear_file[FILE_A];
    }

    Bitboard move = (pos_1 | pos_2) & ~opposing_side & ~own_side;
    Bitboard attack =  ((pos_3 | pos_4) & opposing_side) | (pos_3 | pos_4) & en_passant_target;
    Bitboard valid_pawn_moves = move | attack;

    return valid_pawn_moves;
}

//TODO: whenever promoting occurs, asks two times for promotion. This needs to be fixed.
int promote_pawn(Pieces *own_side, Bitboard start, Bitboard end) {
    int promoting = 1;
    while (promoting) {
        char piece[3];
        puts("Which piece do you want? Input must be 'q', 'r', 'n' or 'b'");
        scanf("%s", piece);
        switch(tolower(piece[0])) {
            case 'q':                      
                own_side->queens |= end;
                promoting = 0;
                break;         
            case 'r':
                own_side->rooks |= end;      
                promoting = 0;
                break;
            case 'n':       
                own_side->knights |= end;
                promoting = 0;    
                break; 
            case 'b':
                own_side->bishops |= end;
                promoting = 0;
                break;
            default:
                puts("Input not valid. Input must be 'q', 'r', 'n' or 'b'."); 
        }
    }
    
    own_side->pawns &= ~start;
    return 0;
}