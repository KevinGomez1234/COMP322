EXECUTABLES=catcher

SRC=catcher.c
OBJ=catcher.o

CC=gcc
CFLAGS=-g -Wall -Wextra

all:$(EXECUTABLES) $(EXECUTABLES)

$(EXECUTABLES): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~ 