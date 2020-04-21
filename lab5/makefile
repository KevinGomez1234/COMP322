EXECUTABLES=paging

SRC=paging.c
OBJ=paging.o

CC=gcc
CFLAGS=-g -Wall -Wextra

all:$(EXECUTABLES) $(EXECUTABLES)

$(EXECUTABLES): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
	rm -f $(EXECUTABLES) $(OBJ) *~