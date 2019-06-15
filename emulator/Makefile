CC = gcc
AR = ar
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = emulator.a

%.o: %.c
	$(CC) -g -c $< -o $@

$(LIB): $(OBJ)
	@cp src/emulator.h .
	$(AR) rcs $@ $^

all: clean
clean: $(LIB)
	@rm $(OBJ) $(LIB) emulator.h
