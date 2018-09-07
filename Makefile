BIN := bin
SRC := src
OBJ := obj
HEADERS := $(wildcard $(SRC)/*.h)
OBJECTS := $(patsubst $(SRC)/%.h, $(OBJ)/%.o, $(HEADERS))

build: program

$(OBJ)/%.o: $(SRC)/%.c 
	gcc -c $< -o $@

program: $(OBJECTS)
	gcc $(OBJECTS) -o $(BIN)/$@

clean:
	-rm $(OBJ)/*
	-rm -f $(BIN)/program

run: $(BIN)/program
	./$(BIN)/program
