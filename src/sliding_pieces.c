#include "bitboard.h"

Bitboard compute_valid_attack(Bitboard loc, char shift_amount, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]) {
    Bitboard valid_moves = 0x0;
    Bitboard pos_1 = loc;
    Bitboard pos_2 = loc;

    // We do this because current piece can land ON the opponent, but not anywhere behind it. Wrapping problem does not exist here.
    Bitboard opp_left_shifted = opposing_side << shift_amount; 
    Bitboard opp_right_shifted = opposing_side >> shift_amount;

    for (int i = 1; i <= RANK_LEN; i++) {
        if (shift_amount == 7) {
            pos_1 = pos_1 & clear_file[FILE_A];
            pos_2 = pos_2 & clear_file[FILE_H];
        } else if (shift_amount == 9 || shift_amount == 1) {
            pos_1 = pos_1 & clear_file[FILE_H];
            pos_2 = pos_2 & clear_file[FILE_A];
        }

        pos_1 = pos_1 << shift_amount & (~own_side) & (~opp_left_shifted); 
        pos_2 = pos_2 >> shift_amount & (~own_side) & (~opp_right_shifted);
        valid_moves = valid_moves | pos_1 | pos_2;
    }
    return valid_moves;
}

Bitboard compute_queen(Bitboard loc, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]) {
    Bitboard valid_diagonal_moves_1 = compute_valid_attack(loc, 7, own_side, opposing_side, clear_file);
    Bitboard valid_diagonal_moves_2 = compute_valid_attack(loc, 9, own_side, opposing_side, clear_file);
    Bitboard valid_horizontal_moves = compute_valid_attack(loc, 1, own_side, opposing_side, clear_file);
    Bitboard valid_vertical_moves = compute_valid_attack(loc, 8, own_side, opposing_side, clear_file);
    return valid_diagonal_moves_1 | valid_diagonal_moves_2 | valid_vertical_moves | valid_horizontal_moves;
}


Bitboard compute_bishop(Bitboard loc, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]) {
    Bitboard valid_diagonal_moves_1 = compute_valid_attack(loc, 7, own_side, opposing_side, clear_file);
    Bitboard valid_diagonal_moves_2 = compute_valid_attack(loc, 9, own_side, opposing_side, clear_file);
    return valid_diagonal_moves_1 | valid_diagonal_moves_2;
}

Bitboard compute_rook(Bitboard loc, Bitboard own_side, Bitboard opposing_side, Bitboard clear_file[FILE_LEN]) {
    Bitboard valid_horizontal_moves = compute_valid_attack(loc, 1, own_side, opposing_side, clear_file);
    Bitboard valid_vertical_moves = compute_valid_attack(loc, 8, own_side, opposing_side, clear_file);
    return valid_vertical_moves | valid_horizontal_moves;
}