EXECUTABLE1=launch 
EXECUTABLE2=tube

SRC1=launch.c
SRC2=tube.c
OBJ1=launch.o
OBJ2=tube.o

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