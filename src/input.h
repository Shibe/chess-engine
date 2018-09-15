#ifndef INPUT_FILE
#define INPUT_FILE

int read_move(int *start_file, int *start_rank, int *end_file, int *end_rank);
int parse_file(char c, int *file);
int parse_rank(char c, int *rank);

#endif
