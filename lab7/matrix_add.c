#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//This doesnt work D:
int main(int argc, char** argv)
{
	if(argc < 3){
		perror("not enough args...");
	}
	else
	{
		int start_time, end_time, scalar, block_size, size, blocks;
		start_time = time(NULL);
		srand(time(NULL));
		scalar = rand();
		size = atoi(argv[1]); 
		blocks = atoi(argv[2]);
		block_size = size / blocks; 
		for(int i = 0; i<block_size*block_size; i++)
		{
			//async read request matrix [x, y]
			//block = async read retun matrix [x,y]
			//asymc write request
			//async write return block
		}

		end_time = time(NULL);
		printf("Time taken: %d\n", end_time - start_time);
	}
}