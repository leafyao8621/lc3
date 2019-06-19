CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = lc3as
LIB = ../assembler/assembler.a

%.o: %.c
	$(CC) -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(BIN)

.PHONY: clean
clean: $(BIN)
	@rm $(OBJ) $(BIN)
