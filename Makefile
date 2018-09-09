CC := gcc
BIN := bin
SRC := src
OBJ := obj
HEADERS := $(wildcard $(SRC)/*.h)
OBJECTS := $(patsubst $(SRC)/%.h, $(OBJ)/%.o, $(HEADERS))

build: program

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) -c $< -o $@

program: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(BIN)/$@

clean:
	-rm $(OBJ)/*
	-rm -f $(BIN)/program

run: $(BIN)/program
	./$(BIN)/program
