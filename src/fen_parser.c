#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "bitboard.h"
#include "chessboard.h"
#include "fen_parser.h"
#include "input.h"
#include "game.h"

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