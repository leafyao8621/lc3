CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = ../emulator/emulator.a ../assembler/assembler.a
BIN = test

%.o: %.c
	$(CC) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(BIN)

.PHONY: clean
clean: $(BIN)
	@rm $(OBJ) $(BIN)
