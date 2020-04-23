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

void eat();
void thinking();
void eat_and_think_cycle();
void initializeSemapores();
void signalHandler();
int phil;
int seats;
int cycles;

//semaphor for each chopstick left and right chopstick
sem_t* chopsticks[100];
char buffer1[100];
char buffer2[100];

int main(int argc, char* argv[])
{
	(void) argc;
//seats
	seats = atoi(argv[1]);
//curent philosopher we start at 1
	phil = atoi(argv[2]) - 1;
	initializeSemapores();
	eat_and_think_cycle();
	return 0;
}

//initializing the chopsticks
void initializeSemapores()
{

	char phil_chop_index_1[32];
	char phil_chop_index_2[32];
	sprintf(phil_chop_index_1, "%d", phil);
	sprintf(phil_chop_index_2, "%d", (phil+1)%seats);
	sprintf(buffer1, "/c_s%s", phil_chop_index_1);
	sprintf(buffer2, "/c_s%s", phil_chop_index_2);
	chopsticks [phil] = sem_open(buffer1, O_CREAT, 0666);
	chopsticks [(phil+1)%seats] = sem_open(buffer2, O_CREAT, 0666);
	if(chopsticks[phil] == SEM_FAILED)
		perror("chop[left]");
	if(chopsticks[(phil+1)%seats] == SEM_FAILED)
		perror("chop[right]");
}

void eat_and_think_cycle()
{
//setup a signal handler to catch SIGTERM and display cycles
	signal(15, signalHandler);
//open up our semaphores to be used with
	do
	{
		sem_wait(chopsticks[phil]);
		sem_wait(chopsticks[(phil+1)%seats]);
		eat();
		sem_post(chopsticks[(phil+1)%seats]);
		sem_post(chopsticks[(phil+1)%seats]);
		cycles++;
		thinking();
	}while(1);
}

void signalHandler(int mysignal)
{
	signal(15, signalHandler);
	if(mysignal == 15)
	{
		sem_close(chopsticks[phil]);
		sem_close(chopsticks[(phil+1)%seats]);
		sem_unlink(buffer1);
		sem_unlink(buffer2);
		sem_destroy(chopsticks[phil]);
		sem_destroy(chopsticks[(phil+1)%seats]);
	}
	fprintf(stderr, "Philosopher%d completed %d cycles\n", phil+1, cycles);
	exit(0);
}

//done
void eat()
{
	srand(time(0));
	int random = rand() % (1000000 + 1 - 0) + 0;
	usleep(random);
	printf("Philosopher %d is eating pid %d\n", phil+1, getpid());
}

void thinking()
{
	srand(time(0));
	int random = rand() % (1000000 + 1 - 0) + 0;
	usleep(random);
	printf("Philosopher %d is thinking pid %d\n", phil+1, getpid());
}
