#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "bitboard.h"
#include "chessboard.h"
#include "fen_parser.h"


int parse(char *fen) {
    Chessboard *chessboard = initialise_chessboard();
    clear_chessboard(chessboard);

    int current_rank = RANK_8; 
    int current_file = FILE_A;
    
    for(; fen[0] != ' ' && fen[0] != '\0'; fen++) {
        char token = fen[0];
        if (token == '/') {
            current_rank--;
            current_file = FILE_A;
        } else if (isdigit(token)) {
            current_file += atoi(&token);
        } else {
            Bitboard location = 1ULL << (current_rank * FILE_LEN + current_file);
            int error = token_to_chessboard(token, location, chessboard);
            if (error) {
                printf("Invalid token %c", token);
                return error;
            }
            current_file++;
        }
    }
    update_chessboard(chessboard);
    print_chessboard(chessboard);
    return 0;
}

int token_to_chessboard(char token, Bitboard location, Chessboard *chessboard) {
    switch(token) {
        case 'P':
            chessboard->white_pieces->pawns |= location;
            return 0;
        case 'R':
            chessboard->white_pieces->rooks |= location;
            return 0;
        case 'N':
            chessboard->white_pieces->knights |= location;
            return 0;
        case 'B':
            chessboard->white_pieces->bishops |= location;
            return 0;
        case 'Q':
            chessboard->white_pieces->queens |= location;
            return 0;
        case 'K':
            chessboard->white_pieces->king |= location;
            return 0;
        case 'p':
            chessboard->black_pieces->pawns |= location;
            return 0;
        case 'r':
            chessboard->black_pieces->rooks |= location;
            return 0;
        case 'n':
            chessboard->black_pieces->knights |= location;
            return 0;
        case 'b':
            chessboard->black_pieces->bishops |= location;
            return 0;
        case 'q':
            chessboard->black_pieces->queens |= location;
            return 0;
        case 'k':
            chessboard->black_pieces->king |= location;
            return 0;
        default:
            return 1;
    }
}