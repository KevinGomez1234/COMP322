EXECUTABLE1=matrix_gen 
EXECUTABLE2=matrix_add

SRC1=matrix_gen.c
SRC2=matrix_add.c
OBJ1=matrix_gen.o
OBJ2=matrix_add.o

CC=gcc
CFLAGS=-g -Wall -Wextra

all:$(EXECUTABLE1) $(EXECUTABLE2)

$(EXECUTABLE1): $(SRC1)
	$(CC) $(CFLAGS) -c $(SRC1)
	$(CC) $(CFLAGS) -o $@ $(OBJ1)

$(EXECUTABLE2): $(SRC2)
	$(CC) $(CFLAGS) -c $(SRC2)
	$(CC) $(CFLAGS) -o $@ $(OBJ2)

clean:
	rm -f $(EXECUTABLE1) $(EXECUTABLE2) $(OBJ1) $(OBJ2) *~ 