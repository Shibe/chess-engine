#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "bitboard.h"
#include "fen_parser.h"

regex_t file_regex;
int result;

void parse(char *fen) {
    // ignore case because prnbqk and PRNBQK both need to match
    result = regcomp(&file_regex, "([prnbqk]|[[:digit:]])", REG_ICASE | REG_EXTENDED);
    if (result) {
        puts("Could not compile regex\n");
        return exit(1);
    }
    Bitboard board = 0x0ULL;
    int rank_count = RANK_LEN;
    char *rank, *str;
    str = strdup(fen);
    while((rank = strsep(&str, "/"))) {
        rank_count--;
        board |= parse_rank(rank, rank_count, board);
    }
    print_board(board);
    free(str);
}

Bitboard parse_rank(char rank[], int rank_count, Bitboard board) {
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
                file_board |= location << (rank_count * 8 + file);
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