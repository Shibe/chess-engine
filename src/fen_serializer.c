#include "chessboard.h"
#include "bitboard.h"
#include "fen_serializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// converts the empty tiles to a single character
int serialize_empty_tiles(int *empty_tiles, char *fen) {
    if (*empty_tiles) {
        char buffer[3];
        sprintf(buffer, "%d", *empty_tiles);
        strcat(fen, buffer);
        *empty_tiles = 0;
    }
    return 0;
}

int serialize_piece(Bitboard active_square, Chessboard *chessboard, char *fen) {
    if (active_square & chessboard->white_pieces->bishops) {
        strcat(fen, "B");
        return 0;
    } else if (active_square & chessboard->white_pieces->king) {
        strcat(fen, "K");
        return 0;
    } else if (active_square & chessboard->white_pieces->knights) {
        strcat(fen, "N");
        return 0;
    } else if (active_square & chessboard->white_pieces->pawns) {
        strcat(fen, "P");
        return 0;
    } else if (active_square & chessboard->white_pieces->queens) {
        strcat(fen, "Q");
        return 0;
    } else if (active_square & chessboard->white_pieces->rooks) {
        strcat(fen, "R");
        return 0;
    } else if (active_square & chessboard->black_pieces->bishops) {
        strcat(fen, "b");
        return 0;
    } else if (active_square & chessboard->black_pieces->king) {
        strcat(fen, "k");
        return 0;
    } else if (active_square & chessboard->black_pieces->knights) {
        strcat(fen, "n");
        return 0;
    } else if (active_square & chessboard->black_pieces->pawns) {
        strcat(fen, "p");
        return 0;
    } else if (active_square & chessboard->black_pieces->queens) { 
        strcat(fen, "q");
        return 0;
    } else if (active_square & chessboard->black_pieces->rooks) {
        strcat(fen, "r");
        return 0;
    }
    return -1;
}

int serialize_chessboard(Chessboard *chessboard, char *fen) {
    int empty_tiles = 0;
    uint64_t active_square = 0x100000000000000ULL;

    while (active_square) {
        uint64_t x = active_square;
        for (int i = 0; i < FILE_LEN; i++) {
            if (chessboard->all_pieces & x) {
                serialize_empty_tiles(&empty_tiles, fen);
                int err = serialize_piece(x, chessboard, fen);
                if (err) {
                    puts("Could not serialize chessboard piece to a character.");
                }
            } else {
                empty_tiles += 1;
            }
            x <<= 1;
        }
        serialize_empty_tiles(&empty_tiles, fen);
        strcat(fen, "/");

        active_square >>= 8;
        
    }
    
    // remove last '/'
    fen[strlen(fen)-1] = 0;
    return 0;
}

int serialize_active_color(Chessboard *chessboard, char *fen) {
    if (chessboard->active_color == WHITE) {
        strcat(fen, "w");
        return 0;
    } else if (chessboard->active_color == BLACK) {
        strcat(fen, "b");
        return 0;
    } else {
        return -1;
    }
}

int serialize_castling(Chessboard *chessboard, char *fen) {
    int castling_options = 0;
    if (chessboard->castle_white & 1) {
        strcat(fen, "K");
        castling_options += 1;
    }
    if (chessboard->castle_white & 2) {
        strcat(fen, "Q");
        castling_options += 1;
    } 
    if (chessboard->castle_black & 1) {
        strcat(fen, "k");
        castling_options += 1;
    } 
    if (chessboard->castle_black & 2) {
        strcat(fen, "q");
        castling_options += 1;
    }    
    if (!castling_options) {
        strcat(fen, "-");
    }

    return 0;    
}

int serialize_file(int file, char *fen) {
    switch(file) {
        case 0:
            strcat(fen, "a");
            return 0;
        case 1:
            strcat(fen, "b");
            return 0;
        case 2:
            strcat(fen, "c");
            return 0;
        case 3:
            strcat(fen, "d");
            return 0;
        case 4:
            strcat(fen, "e");
            return 0;
        case 5:
            strcat(fen, "f");
            return 0;
        case 6:
            strcat(fen, "g");
            return 0;
        case 7:
            strcat(fen, "h");
            return 0;
        default:
            return -1;
    }
}

void serialize_rank(int rank, char *fen) {
    char c_rank[3];
    sprintf(c_rank, "%d", rank);
    strcat(fen, c_rank);
}

int serialize_en_passant_target(Chessboard *chessboard, char *fen) {
    Bitboard en_passant_target = chessboard->en_passant_target;    
    int counter = 0;
    
    if (en_passant_target) {
        while (en_passant_target) {
            en_passant_target >>= 1;
            counter++;
        }

        int file = (counter % 8) - 1;
        int err = serialize_file(file, fen);
        if (err) {
            puts("Could not serialize file.");
        }

        int rank = (counter / 8) + 1;
        serialize_rank(rank, fen);

        return 0;
    }
    
    // there is no en pessant target
    strcat(fen, "-");

    return 0;  
}

int serialize_halfmove_clock(Chessboard *chessboard, char *fen) {
    int halfmove_clock = chessboard->halfmove_clock;
    char buffer[4];
    sprintf(buffer, "%d", halfmove_clock);
    strcat(fen, buffer);

    return 0;
}

int serialize_fullmove_number(Chessboard *chessboard, char *fen) {
    int fullmove_number = chessboard->fullmove_number;
    char buffer[4];
    sprintf(buffer, "%d", fullmove_number);
    strcat(fen, buffer);

    return 0;
}

int serialize_fen(Chessboard *chessboard, char **target) {
    
    char *fen = calloc(128, sizeof(char));
    
    int err = serialize_chessboard(chessboard, fen);
    if (err) {
        puts("Could not serialize chessboard.");
        return err;
    }
    strcat(fen, " ");
    err = serialize_active_color(chessboard, fen);
    if (err) {
        puts("Could not serialize active color");
        return err;
    }
    strcat(fen, " ");
    err = serialize_castling(chessboard, fen);
    if (err) {
        puts("Could not serialize castling");
        return err;
    }
    strcat(fen, " ");
    err = serialize_en_passant_target(chessboard, fen);
    if (err) {
        puts("Could not serialize en passant target");
        return err;
    }
    strcat(fen, " ");
    err = serialize_halfmove_clock(chessboard, fen);
    if (err) {
        puts("Could not serialize halfmove clock");
        return err;
    }
    strcat(fen, " ");
    err = serialize_fullmove_number(chessboard, fen);
    if (err) {
        puts("Could not serialize fullmove number");
        return err;
    }

    *target = fen;
    return 0;
}