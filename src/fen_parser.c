#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bitboard.h"
#include "chessboard.h"
#include "fen_parser.h"
#include "input.h"
#include "game.h"

int convert_empty_tiles(int *empty_tiles, char *fen) {
    if (*empty_tiles) {
        char buffer[2];

        itoa(*empty_tiles, buffer, 16);
        strcat(fen, buffer);
        *empty_tiles = 0;
    }
    return 0;
}

int reverse_parse_pieces(Chessboard *chessboard, char *fen) {
    int empty_tiles = 0;
    uint64_t original = 0x100000000000000ULL;

    while (original) {
        uint64_t x = original;

        for (int i = 0; i < FILE_LEN; i++) {
            if (x & chessboard->white_pieces->bishops) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "B");
            } else if (x & chessboard->white_pieces->king) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "K");
            } else if (x & chessboard->white_pieces->knights) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "N");
            } else if (x & chessboard->white_pieces->pawns) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "P");
            } else if (x & chessboard->white_pieces->queens) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "Q");
            } else if (x & chessboard->white_pieces->rooks) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "R");
            } else if (x & chessboard->black_pieces->bishops) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "b");
            } else if (x & chessboard->black_pieces->king) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "k");
            } else if (x & chessboard->black_pieces->knights) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "n");
            } else if (x & chessboard->black_pieces->pawns) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "p");
            } else if (x & chessboard->black_pieces->queens) { 
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "q");
            } else if (x & chessboard->black_pieces->rooks) {
                convert_empty_tiles(&empty_tiles, fen);
                strcat(fen, "r");
            } else {
                empty_tiles += 1;
            }   

            x <<= 1;
        }

        convert_empty_tiles(&empty_tiles, fen);
        strcat(fen, "/");

        original >>= 8;
    }

    // remove last '/'
    fen[strlen(fen)-1] = 0;

    return 0;
}

int reverse_parse_active_color(Chessboard *chessboard, char *fen) {
    if (chessboard->active_color == WHITE) {
        strcat(fen, "w");
        return 0;
    } else if (chessboard->active_color == BLACK) {
        strcat(fen, "b");
        return 0;
    } else {
        return 1;
    }
}

int reverse_parse_castling(Chessboard *chessboard, char *fen) {
    if (chessboard->castle_white & 1) {
        strcat(fen, "K");
    } else {
        strcat(fen, "-");
    }  
    if (chessboard->castle_white & 2) {
        strcat(fen, "Q");
    } else {
        strcat(fen, "-");
    }
    if (chessboard->castle_black & 1) {
        strcat(fen, "k");
    } else {
        strcat(fen, "-");
    }
    if (chessboard->castle_black & 2) {
        strcat(fen, "q");
    } else {
        strcat(fen, "-");
    }

    return 0;    
}

void reverse_parse_file(int file, char *fen) {
    switch(file) {
        case 0:
            strcat(fen, "a");
            break;
        case 1:
            strcat(fen, "b");
            break;
        case 2:
            strcat(fen, "c");
            break;
        case 3:
            strcat(fen, "d");
            break;
        case 4:
            strcat(fen, "e");
            break;
        case 5:
            strcat(fen, "f");
            break;
        case 6:
            strcat(fen, "g");
            break;
        case 7:
            strcat(fen, "h");
            break;
    }
}

void reverse_parse_rank(int rank, char *fen) {
    char c_rank[2];
    itoa(rank, c_rank, 16); 
    strcat(fen, c_rank);
}

int reverse_parse_en_pessant_target(Chessboard *chessboard, char *fen) {
    char *c_pos = malloc(3 * sizeof(char));
    Bitboard en_pessant_target = chessboard->en_passant_target;    
    int counter = 0;
    
    if (en_pessant_target) {
        while (en_pessant_target) {
            en_pessant_target >>= 1;
            counter++;
        }

        int file = (counter % 8) - 1;
        reverse_parse_file(file, fen);

        int rank = (counter / 8) + 1;
        reverse_parse_rank(rank, fen);

        return 0;
    }
    
    // there is no en pessant target
    strcat(fen, "-");

    return 0;  
}

int reverse_parse_halfmove_clock(Chessboard *chessboard, char *fen) {
    int halfmove_clock = chessboard->halfmove_clock;
    char buffer[2];
    itoa(halfmove_clock, buffer, 16);
    strcat(fen, buffer);

    return 0;
}

int reverse_parse_fullmove_number(Chessboard *chessboard, char *fen) {
    int fullmove_number = chessboard->fullmove_number;
    char buffer[2];
    itoa(fullmove_number, buffer, 16);
    strcat(fen, buffer);

    return 0;
}

int create_fen(Chessboard *chessboard, char **target) {
    
    char *fen = calloc(128, sizeof(char));
    
    int err = reverse_parse_pieces(chessboard, fen);
    if (err) {
        printf("Could not reverse parse pieces.");
        return err;
    }
    strcat(fen, " ");
    err = reverse_parse_active_color(chessboard, fen);
    if (err) {
        printf("Could not reverse parse active color");
        return err;
    }
    strcat(fen, " ");
    err = reverse_parse_castling(chessboard, fen);
    if (err) {
        printf("Could not reverse parse castling");
        return err;
    }
    strcat(fen, " ");
    err = reverse_parse_en_pessant_target(chessboard, fen);
    if (err) {
        printf("Could not reverse parse en passant target");
        return err;
    }
    strcat(fen, " ");
    err = reverse_parse_halfmove_clock(chessboard, fen);
    if (err) {
        printf("Could not reverse parse halfmove clock");
        return err;
    }
    strcat(fen, " ");
    err = reverse_parse_fullmove_number(chessboard, fen);
    if (err) {
        printf("Could not reverse parse fullmove number");
        return err;
    }

    *target = fen;
    return 0;
}

int parse(Chessboard *chessboard, char *fen) {
    char **stream = &fen;
    int err = parse_pieces(chessboard, stream);
    if (err) {
        printf("Could not parse pieces. Found char: %c\n", fen[0]);
        return err;
    }
    parse_whitespace(stream);
    err = parse_active_color(chessboard, stream);
    if (err) {
        printf("Could not parse active color, found char: %c\n", fen[0]);
        return err;
    }
    parse_whitespace(stream);
    err = parse_castling(chessboard, stream);
    if (err) {
        printf("Could not parse castling, found char: %c\n", fen[0]);
        return err;
    }
    parse_whitespace(stream);
    err = parse_en_passant_target(chessboard, stream);
    if (err) {
        printf("Could not parse en passant target, found char: %c\n", fen[0]);
        return err;
    }
    parse_whitespace(stream);
    err = parse_halfmove_clock(chessboard, stream);
    if (err) {
        printf("Could not parse halfmove clock, found char: %c\n", fen[0]);
        return err;
    }
    parse_whitespace(stream);
    err = parse_fullmove_number(chessboard, stream);
    if (err) {
        printf("Could not parse fullmove number, found char: %c\n", fen[0]);
        return err;
    }

    update_chessboard(chessboard);
    return 0;
}

int parse_whitespace(char **stream) {
    while (**stream == ' ') {
        (*stream)++;
    }
    return 0;
}

// 1 -> left shift (7 * 8 + 0)

int parse_pieces(Chessboard *chessboard, char **stream) {
    int current_rank = RANK_8; 
    int current_file = FILE_A;

    char token = **stream;
    while(token != ' ' && token != '\0') {
        if (token == '/') {
            current_rank--;
            current_file = FILE_A;
        } else if (isdigit(token)) {
            current_file += atoi(&token);
        } else {
            Bitboard location = 1ULL << (current_rank * FILE_LEN + current_file);
            int error = token_to_chessboard(token, location, chessboard);
            if (error) {
                return error;
            }
            current_file++;
        }
        (*stream)++;
        token = **stream;
    }
    return 0;
}

int parse_active_color(Chessboard *chessboard, char **stream) {
    char c = **stream;
    switch(c) {
        case 'w':
            chessboard->active_color = WHITE;
            break;
        case 'b':
            chessboard->active_color = BLACK;
            break;
        default:
            return 1;
    }
    (*stream)++;
    return 0;
}

int parse_castling(Chessboard *chessboard, char **stream) {
    char token = **stream;
    while(token != ' ' && token != '\0') {
        switch(token) {
            case '-':
                break;
            case 'K':
                chessboard->castle_white = chessboard->castle_white | SHORT;
                break;
            case 'Q':
                chessboard->castle_white = chessboard->castle_white | LONG;
                break;
            case 'k':
                chessboard->castle_black = chessboard->castle_black | SHORT;
                break;
            case 'q':
                chessboard->castle_black = chessboard->castle_black | LONG;
                break;
            default:
                return 1;
        }
        (*stream)++;
        token = **stream;
    }
    return 0;
}

int parse_en_passant_target(Chessboard *chessboard, char **stream) {
    if (**stream == '-') {
        chessboard->en_passant_target = 0x0ULL;
        (*stream)++;
        return 0;
    }

    int file, rank;
    int success = parse_file(**stream, &file);
    if (!success) {
        return 1;
    }
    (*stream)++;
    success = parse_rank(**stream, &rank);
    if (!success) {
        return 1;
    }
    (*stream)++;
    Bitboard target = input_to_square(rank, file);
    chessboard->en_passant_target = target;
    return 0;
}

int parse_halfmove_clock(Chessboard *chessboard, char **stream) {
    char token = **stream;
    if (isdigit(token)) {
        chessboard->halfmove_clock = atoi(&token);
        (*stream)++;
        return 0;
    }
    return 1;
}

int parse_fullmove_number(Chessboard *chessboard, char **stream) {
    char token = **stream;
    if (isdigit(token)) {
        chessboard->fullmove_number = atoi(&token);
        (*stream)++;
        return 0;
    }
    return 1;
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