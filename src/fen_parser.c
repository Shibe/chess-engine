#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bitboard.h"
#include "chessboard.h"
#include "fen_parser.h"

regex_t file_regex;
int result;

void parse(char *fen) {
    // ignore case because prnbqk and PRNBQK both need to match
    result = regcomp(&file_regex, "([prnbqk]|[[:digit:]])", REG_ICASE | REG_EXTENDED);
    if (result) {
        puts("Couldn't compile regex\n");
        return exit(1);
    }
    Bitboard board = 0x0ULL;
    int rank_count = RANK_LEN;
    char *rank, *str;
    str = strdup(fen);
    //TODO: refactor
    Chessboard *chessboard = initialise_chessboard();
    clear_chessboard(chessboard);
    while((rank = strsep(&str, "/"))) {
        rank_count--;
        board |= parse_rank(rank, rank_count, chessboard);
    }
    update_chessboard(chessboard);
    print_board(chessboard->all_pieces);
    free(str);
}

Bitboard parse_rank(char rank[], int rank_count, Chessboard *board) {
    int file = 0;
    int shift_amount;
    Bitboard file_board = 0x0ULL;
    for (int i = 0; rank[i] != 0; i++) {
        char token = rank[i];
        result = regexec(&file_regex, &token, 0, NULL, 0);
        if (!result) { // match found
            shift_amount = get_shift_amount(token);
            file = file + shift_amount;
            if (!shift_amount) {
                Bitboard location = 0x00000000000001ULL;
                location = location << (rank_count * 8 + file);
                token_to_chessboard(token, location, board);
                file++;
            }
        }
        else if (result == REG_NOMATCH) {
            printf("'%s' doesn't match \n", &token);
        }
    }
    return file_board;
}

int get_shift_amount(char token) {
    if (token >= '0' && token <= '9') {
        return atoi(&token);
    } else {
        return 0;
    }
}

void token_to_chessboard(char token, Bitboard location, Chessboard *chessboard) {
    switch(token) {
        case 'p':
            chessboard->white_pawns |= location;
            return;
        case 'r':
            chessboard->white_rooks |= location;
            return;
        case 'n':
            chessboard->white_knights |= location;
            return;
        case 'b':
            chessboard->white_bishops |= location;
            return;
        case 'q':
            chessboard->white_queens |= location;
            return;
        case 'k':
            chessboard->white_king |= location;
            return;
        case 'P':
            chessboard->black_pawns |= location;
            return;
        case 'R':
            chessboard->black_rooks |= location;
            return;
        case 'N':
            chessboard->black_knights |= location;
            return;
        case 'B':
            chessboard->black_bishops |= location;
            return;
        case 'Q':
            chessboard->black_queens |= location;
            return;
        case 'K':
            chessboard->black_king |= location;
            return;
    }
}