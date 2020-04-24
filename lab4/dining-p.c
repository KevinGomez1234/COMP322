#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

//This program is meant to run many times simultaneously. Every process is a philosopher competing for a chopstick resource. 
//Removed hold and wait condition to remove deadlock by not picking up any chopsticks if either left or right chopstick is being held.

void eat();
void thinking();
void eat_and_think_cycle();
void initializeSemapores();
void signalHandler();

int phil;
int seats;
int cycles;
//semaphor for each chopstick left and right chopstick
sem_t* chopsticks[2];
char buffer1[50];
char buffer2[50];

int main(int argc, char* argv[])
{
	//error checking and logic
	if(argc < 3)
	{
		printf("not enough args: ./dining-p <seats> <position>\n");
	}

	else if(argc >= 3)
	{
		//since we start counting philosophers from 0 subtract 1 from seats.
		seats = atoi(argv[1])-1;
		//Phil position 0 through N. 
		phil = atoi(argv[2]);
		//phil position is greater than seats available error, don't initialize chopsticks, exit with error flag.
		if(phil > seats)
		{
			printf("not enough seats\n");
			exit(1);
		}
		//else initialize semaphores and start eat/think cycle.
		else
		{
			initializeSemapores();
			eat_and_think_cycle();
		}

	}
	return 0;
}

//initializing the chopsticks
void initializeSemapores()
{
	char phil_chop_index_1[32];
	char phil_chop_index_2[32];
	sprintf(phil_chop_index_1, "%d", phil);
	sprintf(phil_chop_index_2, "%d", (phil+1)%seats);
	//we have a char buffer that maps to the sem name
	buffer1[0] = '/'; buffer1[1] = 'c'; strcat(buffer1, phil_chop_index_1);
	buffer2[0] = '/'; buffer2[1] = 'c'; strcat(buffer2, phil_chop_index_2);

	//open the buffers
	chopsticks[0] = sem_open(buffer1, O_CREAT, 0666, 1);
	chopsticks[1] = sem_open(buffer2, O_CREAT, 0666, 1);
	//check chopsticks output
	if(chopsticks[0]==SEM_FAILED)
		perror("chop[0] failed");
	if(chopsticks[1]==SEM_FAILED)
		perror("chop[1] failed");	
}

void eat_and_think_cycle()
{
	//setup a signal handler to catch SIGTERM and display cycles
	signal(15, signalHandler);
	//open up our semaphores to be used with
	do
	{
		//used to store value of semaphores to prevent deadlock.
		int sem_val_chopstick1;
		int sem_val_chopstick2;
		//check if chopsticks are being used (if either sem_values == 0). If either left or right chopstick is being used then don't pick any of them up, else pick both up.
		sem_getvalue(chopsticks[0], &sem_val_chopstick1);
		sem_getvalue(chopsticks[1], &sem_val_chopstick2); 
		//loop until both semaphores are available, this prevents deadlock by removing hold and wait.
		if(sem_val_chopstick1 == 0 || sem_val_chopstick2 == 0)
			continue;
		else
		{
			sem_wait(chopsticks[0]);
			sem_wait(chopsticks[1]);	
			eat();
			sem_post(chopsticks[0]);
			sem_post(chopsticks[1]);
			cycles++;
			thinking();
		}

	}while(1);
}

//terminate program and close all semaphores properly.
void signalHandler(int mysignal)
{
	signal(15, signalHandler);
	if(mysignal == 15)
	{
		sem_close(chopsticks[0]);
		sem_close(chopsticks[1]);
		sem_unlink(buffer1);
		sem_unlink(buffer2);
		//sem_destroy(chopstick[i]) used for unamed semaphores.
	}
	fprintf(stderr, "Philosopher%d completed %d cycles\n", phil, cycles);
	exit(0);
}

//Eat and think functions. 
void eat()
{
	srand(time(0));
	int random = rand() % (1000000 + 1 - 0) + 0;
	usleep(random);
	printf("Philosopher %d is eating.\n", phil);
}

void thinking()
{
	srand(time(0));
	int random = rand() % (1000000 + 1 - 0) + 0;
	usleep(random);
	printf("Philosopher %d is thinking.\n", phil);
}