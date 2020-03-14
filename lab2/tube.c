#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>	

void logic();
int indexAtComma();
void runArgs();

int main(int argc, char** argv)
{
	logic(argc, argv);
	return 0;
}

//logic for the program
void logic(int argc, char* argv[])
{
	//split the command line arguments at the comma. 
	int split = indexAtComma(argc, argv);
	if(split < 0)
	{
		printf("Improper Usage, use like this: ./tube <arg1> , <arg2>\n");
	}

	else
	{
		runArgs(argc, argv, split);
	}
}

int indexAtComma(int argc, char* argv[])
{	
	int count = 0;
	//parse the command line args to find the comma
	while(count < argc)
	{
		if(argv[count][0] == ',')
			break;
		count++;
	}	
	//if there are less than 3 args you're missing a comma or an argument, or if one side of comma has no args you're missing an argument.
	if (argc < 3 || count == argc-1 || count == argc)
	{
		count = -1;
	}
	return count;
}

void runArgs(int argc, char* argv[], int count)
{
	pid_t childA, childB;
	int statusA, statusB, pipe_return_value, the_pipe[2];
	char* args1[count+1];
	char* args2[argc-count];
	int counter_2 = 0;

	for(int i = 1; i<count; i++)
	{
		args1[i-1] = argv[i];
	}	
	args1[count-1] = NULL;

	for(int j = count+1; j<argc; j++)
	{
		args2[counter_2] = argv[j];
		counter_2++;
	}

	args2[counter_2+1] = NULL;
	//another function
	//make the pipe
	pipe_return_value = pipe(the_pipe);
	//make fork a child process
	childA = fork();
	if(pipe_return_value == -1)
	{
		perror("initializing pipe did not work");
		exit(1);
	}
	else
	{
		//child A will output something, so we will redirect the standard out into a file[]
		if((int)childA == 0)
		{
			close(the_pipe[0]);
			dup2(the_pipe[1], STDOUT_FILENO);
			execve(args1[0], args1, NULL);
		}

		else if((int)childA > 0)
		{			
			childB = fork();
			if((int)childB == 0)
			{
				close(the_pipe[1]);
				dup2(the_pipe[0], STDIN_FILENO);
				execve(args2[0], args2, NULL);
			}
			else if((int)childB > 0)
			{
				fprintf(stderr, "%s $$ =  %d\n", args1[0],(int)childA);
				fprintf(stderr, "%s $$ = %d\n", args2[0],(int)childB);
				waitpid((int)childA, &statusA, 0);
				waitpid((int)childB, &statusB, 0);
				close(the_pipe[1]);
				close(the_pipe[0]);
				fprintf(stderr, "%s $? = %d\n%s $? = %d\n", args1[0], WEXITSTATUS(statusA), args2[0], WEXITSTATUS(statusB));
			}
		}
	}
}