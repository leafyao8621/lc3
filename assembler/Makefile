CC = gcc
AR = ar
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB = assembler.a

%.o: %.c
	$(CC) -g -c $< -o $@

$(LIB): $(OBJ)
	@cp src/assembler.h .
	$(AR) rcs $@ $^

all: clean
clean: $(LIB)
	@rm $(OBJ) $(LIB) assembler.h
