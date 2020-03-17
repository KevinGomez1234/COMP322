#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void logic();
int main(int argc, char* argv[])
{
	logic(argc, argv);
	return 0;
}

void logic(int argc, char* argv[])
{
	fprintf(stderr, "%s: $$ = %d \n", argv[0],getpid());
	
}