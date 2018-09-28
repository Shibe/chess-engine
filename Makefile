CC := gcc
BIN := bin
SRC := src
OBJ := obj
C_FILES := $(wildcard $(SRC)/*.c)
OBJECTS_TEST:= $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(filter-out $(SRC)/main.c, $(C_FILES)))
OBJECTS_BUILD := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(filter-out $(wildcard $(SRC)/*_tests.c), $(C_FILES)))

build: clean program

program: $(OBJECTS_BUILD)
	$(CC) -g $(OBJECTS_BUILD) -o $(BIN)/$@

test: clean program_test

program_test: $(OBJECTS_TEST)
	$(CC) -g $(OBJECTS_TEST) -o $(BIN)/program

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -g -c $< -o $@

clean:
	-rm $(OBJ)/*
	-rm -f $(BIN)/program

run: $(BIN)/program
	./$(BIN)/program
