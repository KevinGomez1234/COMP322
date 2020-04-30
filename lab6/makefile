EXECUTABLE1=spiritd 
EXECUTABLE2=mole
LOGFILE = lab6.log

SRC1=spiritd.c
SRC2=mole.c
OBJ1=spiritd.o
OBJ2=mole.o

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
	rm -f $(EXECUTABLE1) $(EXECUTABLE2)  $(OBJ1) $(OBJ2) *~ 