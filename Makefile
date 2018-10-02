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

clean:
	-rm $(OBJ)/*
	-rm -f $(BIN)/program

run: $(BIN)/program
	./$(BIN)/program

test: clean-test program-test

program-test: $(OBJECTS_TEST)
	$(CC) -g $(OBJECTS_TEST) -o $(BIN)/test

clean-test:
	-rm $(OBJ)/*
	-rm -f $(BIN)/test

run-test: $(BIN)/test
	./$(BIN)/test

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -g -c $< -o $@

