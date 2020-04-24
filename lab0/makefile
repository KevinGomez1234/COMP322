EXECUTABLES=zero-one
SRC=zero-one.c
OBJ=zero-one.o

CC=gcc
CFLAGS= -g -Wall -Wextra

all:$(EXECUTABLES)

$(EXECUTABLES): $(SRC)
        $(CC) $(CFLAGS) -c $(SRC)
        $(CC) $(CFLAGS) -o $@ $(OBJ) -lm

clean:
        rm -f $(EXECUTABLES) $(OBJ) *~ 
