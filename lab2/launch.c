#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void logic();
int main(int argc, char** argv)
{
	//to ignore the unused parameter warning.
	(void)argc;
	logic(argv);
	return 0;
}


void logic(char** argv)
{
	int status = 0;
	int returned;
	pid_t process_1 = fork();
	
	// we are the child
	if((int)process_1 == 0)
	{
		execve(argv[1], argv+1, NULL);
		exit(0);
	}
	else if((int)process_1 > 0)
	{	
		fprintf(stderr ,"%s: $$ = %d\n", argv[1], process_1);
		waitpid(process_1, &status, 0);
		returned = WEXITSTATUS(status);
		fprintf(stderr, "%s: $? = %d\n", argv[1], returned);
	}

}

