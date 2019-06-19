CC = gcc
LIB = ../assembler/assembler.a ../emulator/emulator.a
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
GTK = `pkg-config --cflags --libs gtk+-3.0`
BIN = lc3sim-gtk

%.o: %.c
	$(CC) -c $< -o $@ $(GTK)

$(BIN): $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(BIN) $(GTK)

