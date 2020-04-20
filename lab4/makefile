EXECUTABLES=dining-p

SRC=dining-p.c
OBJ=dining-p.o

CC=gcc
CFLAGS=-g -Wall -Wextra -lpthread 

all:$(EXECUTABLES) $(EXECUTABLES)

$(EXECUTABLES): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~