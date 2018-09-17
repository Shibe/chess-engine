#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "bitboard.h"
#include "chessboard.h"
#include "fen_parser.h"

regex_t FILE_REGEX;

int parse(char *fen) {
    Chessboard *chessboard = initialise_chessboard();
    clear_chessboard(chessboard);

    // ignore case because prnbqk and PRNBQK both need to match
    int reg_error = regcomp(&FILE_REGEX, "([prnbqk]|[[:digit:]])", REG_ICASE | REG_EXTENDED);
    if (reg_error) {
        puts("Couldn't compile regex\n");
        return 1;
    }

    char *rank, *str;
    str = strdup(fen);
    for(int curr_rank = RANK_8; curr_rank >= RANK_1; curr_rank--) {
        rank = strsep(&str, "/");
        parse_rank_token(rank, curr_rank, chessboard);
    }

    update_chessboard(chessboard);
    print_board(chessboard->all_pieces);
    free(str);
    return 0;
}

int parse_rank_token(char rank[], int current_rank, Chessboard *board) {
    int file = 0;
    for (int i = 0; rank[i] != '\0'; i++) {
        char c = rank[i];
        int reg_error = regexec(&FILE_REGEX, &c, 0, NULL, 0);
        if (reg_error) {
            if (reg_error == REG_NOMATCH) {
                printf("'%s' doesn't match \n", &c);
            }
            return 1;
        }
        if (isdigit(c)) {
            file = file + atoi(&c);
        } else {
            Bitboard location = 1ULL << (current_rank * 8 + file);
            token_to_chessboard(c, location, board);
            file++;
        }
    }
    return 0;
}

void token_to_chessboard(char token, Bitboard location, Chessboard *chessboard) {
    switch(token) {
        case 'P':
            chessboard->white_pawns |= location;
            return;
        case 'R':
            chessboard->white_rooks |= location;
            return;
        case 'N':
            chessboard->white_knights |= location;
            return;
        case 'B':
            chessboard->white_bishops |= location;
            return;
        case 'Q':
            chessboard->white_queens |= location;
            return;
        case 'K':
            chessboard->white_king |= location;
            return;
        case 'p':
            chessboard->black_pawns |= location;
            return;
        case 'r':
            chessboard->black_rooks |= location;
            return;
        case 'n':
            chessboard->black_knights |= location;
            return;
        case 'b':
            chessboard->black_bishops |= location;
            return;
        case 'q':
            chessboard->black_queens |= location;
            return;
        case 'k':
            chessboard->black_king |= location;
            return;
    }
}