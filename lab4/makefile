EXECUTABLES=dining-p

SRC=dining-p.c
OBJ=dining-p.o

CC=gcc
CFLAGS=-g -lpthread -lrt -Wall -Wextra

all:$(EXECUTABLES) $(EXECUTABLES)

$(EXECUTABLES): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lpthread -lrt

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~ 
