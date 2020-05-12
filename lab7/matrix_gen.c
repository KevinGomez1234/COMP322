#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv)
{
	if(argc < 2)
		perror("Not enough args, use like this ./matrix_gen size <file>\n");
	else
	{
		int size = atoi(argv[1]);
		int random;						
		srand(time(NULL));
		for(int i = 0; i<size; i++)
		{
			for(int j = 0; j<size; j++)
			{
				//get a random number between 100 and negative 100
				random = (rand() % 201) - 100;
				fprintf(stdout, "%d ", random);
			}
			fprintf(stdout, "\n");
		}
	}
}