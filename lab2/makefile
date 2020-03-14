all: tube launch
tube: tube.o
	gcc tube.o -o tube -Wall -Wextra
tube.o: tube.c
	gcc -c tube.c -Wall -Wextra
launch: launch.o 
	gcc launch.o -o launch -Wall -Wextra
launch.o: launch.c
	gcc -c launch.c -Wall -Wextra
clean:
	-rm *.o $(objects) launch tube