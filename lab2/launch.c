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
	logic(argc, argv);
	return 0;
}

void logic(int argc, char** argv)
{
	int status = 0;
	int returned;

	//improper usage case. need at least 1 arg. 
	if (argc < 2)
	{
		printf("Improper usage, use like this: ./launch <cmd>\n");
	}
	//run the program
	else
	{
		pid_t process_1 = fork();
		// we are the child, execute the argument.
		if((int)process_1 == 0)
		{
			execve(argv[1], argv+1, NULL);
			exit(0);
		}
		//parent prints pid and return status
		else if((int)process_1 > 0)
		{	
			fprintf(stderr ,"%s: $$ = %d\n", argv[1], process_1);
			waitpid(process_1, &status, 0);
			returned = WEXITSTATUS(status);
			fprintf(stderr, "%s: $? = %d\n", argv[1], returned);
		}
	}

}