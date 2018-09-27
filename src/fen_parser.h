#include "bitboard.h"

int parse_fen(Chessboard *chessboard, char *fen);
int parse_whitespace(char **stream);
int parse_pieces(Chessboard *chessboard, char **stream);
int parse_active_color(Chessboard *chessboard, char **stream);
int parse_castling(Chessboard *chessboard, char **stream);
int parse_en_passant_target(Chessboard *chessboard, char **stream);
int parse_halfmove_clock(Chessboard *chessboard, char **stream);
int parse_fullmove_number(Chessboard *chessboard, char **stream);
int token_to_chessboard(char token, Bitboard location, Chessboard *chessboard);