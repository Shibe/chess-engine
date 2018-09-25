#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bitboard.h"
#include "chessboard.h"
#include "fen_parser.h"
#include "input.h"
#include "game.h"


// int generate_fen_string(Chessboard *chessboard, char *fen){
//     uint64_t x = 0x0000000000000001ULL;
//     int y = 0;

//     while (x) {
//         if (x & chessboard->white_pieces->bishops) {
//             fen[y] = 'B';
//         } else if (x & chessboard->white_pieces->king) {
//             fen[y] = 'K';
//         } else if (x & chessboard->white_pieces->knights) {
//             fen[y] = 'N';
//         } else if (x & chessboard->white_pieces->pawns) {
//             fen[y] = 'P';
//         } else if (x & chessboard->white_pieces->queens) {
//             fen[y] = 'Q';
//         } else if (x & chessboard->white_pieces->rooks) {
//             fen[y] = 'R';
//         } else if (x & chessboard->black_pieces->bishops) {
//             fen[y] = 'b';
//         } else if (x & chessboard->black_pieces->king) {
//             fen[y] = 'k';
//         } else if (x & chessboard->black_pieces->knights) {
//             fen[y] = 'n';
//         } else if (x & chessboard->black_pieces->pawns) {
//             fen[y] = 'p';
//         } else if (x & chessboard->black_pieces->queens) { 
//             fen[y] = 'q';
//         } else if (x & chessboard->black_pieces->rooks) {
//             fen[y] = 'r';
//         } else {
//             fen[y] = '0';
//         }  

//         if (!((y+2) % 9)) {
//             y++;
//             fen[y] = '/';
//         } 

//         x <<= 1;
//         y++;
//     }

//     fen[y] = '\0';

//     // for testing purposes
//     for (int i = 0; i < 71; i++){
//         printf("%c", fen[i]);
       
//     }

//     return 0;
// }

int convert_empty_tiles(int *empty_tiles, char *fen) {
    if (*empty_tiles) {
        char buffer[2];

        itoa(*empty_tiles, buffer, 16);
        strcat(fen, buffer);
        *empty_tiles = 0;
    }
    return 0;
}

// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

int reverse_parse_pieces(Chessboard *chessboard, char *fen) {
    int empty_tiles = 0;
    uint64_t x = 0x0000000000000001ULL;
    
    while (x) {      
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

        if (!(clear_file[FILE_H] & x)){
            convert_empty_tiles(&empty_tiles, fen);
            strcat(fen, "/");
        }

        x <<= 1;    
    }

    // remove last '/'
    fen[strlen(fen)-1] = 0;
    strcat(fen, " ");

    return 0;
}

int reverse_parse_active_color(Chessboard *chessboard, char *fen) {
    if (chessboard->active_color == WHITE) {
        strcat(fen, "w");
    } else {
        strcat(fen, "b");
    }
    return 0;
}

int reverse_parse_castling(Chessboard *chessboard, char *fen) {
    return 0;    
}

int reverse_parse_en_pessant_target(Chessboard *chessboard, char *fen) {
    return 0;  
}

int reverse_parse_halfmove_clock(Chessboard *chessboard, char *fen) {
    return 0;
}

int reverse_parse_fullmove_number(Chessboard *chessboard, char *fen) {
    return 0;
}

int reverse_parse(Chessboard *chessboard, char *fen) {
    
    fen = calloc(124, sizeof(char));
    
    int err = reverse_parse_pieces(chessboard, fen);
    if (err) {
        printf("Could not reverse parse pieces.");
        return err;
    }
    err = reverse_parse_active_color(chessboard, fen);
    if (err) {
        printf("Could not reverse parse active color");
        return err;
    }

    err = reverse_parse_castling(chessboard, fen);
    if (err) {
        printf("Could not reverse parse castling");
        return err;
    }
    err = reverse_parse_en_pessant_target(chessboard, fen);
    if (err) {
        printf("Could not reverse parse en passant target");
        return err;
    }
    err = reverse_parse_halfmove_clock(chessboard, fen);
    if (err) {
        printf("Could not reverse parse halfmove clock");
        return err;
    }
    err = reverse_parse_fullmove_number(chessboard, fen);
    if (err) {
        printf("Could not reverse parse fullmove number");
        return err;
    }

    // for testing purposes
    for (int i = 0; i < 71; i++){
        printf("%c", fen[i]);     
    }

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