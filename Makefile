CC := gcc
BIN := bin
SRC := src
OBJ := obj
HEADERS := $(wildcard $(SRC)/*.c)
OBJECTS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(HEADERS))

build: clean program

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -g -c $< -o $@

program: $(OBJECTS)
	$(CC) -g $(OBJECTS) -o $(BIN)/$@

clean:
	-rm $(OBJ)/*
	-rm -f $(BIN)/program

run: $(BIN)/program
	./$(BIN)/program
