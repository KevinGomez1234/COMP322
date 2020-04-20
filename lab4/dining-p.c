#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#define SEMA "/CHOPSTICK"
#define C1 "/CHOPSTICK1"
#define C2 "/CHOPSTICK2"

void eat();
void thinking();
void eat_and_think_cycle();
void signalHandler();
int phil;
int seats;
int cycles;

sem_t* chopstick1;
sem_t* chopstick2;

sem_t* chopstick;

int main(int argc, char* argv[])
{
	//argv[1] = seats, argv[2] = positions
	(void) argc;
	int elements = atoll(argv[1]);
	chopstick = malloc(elements * sizeof(chopstick[0]));
	seats = elements;
	//our philosophers start from 0
	phil = atoll(argv[2]) - 1;
	
	chopstick[phil] = sem_init(chopstick1, 1, 1);
	chopstick[(phil+1)] = sem_init(chopstick2, 1, 1);

	eat_and_think_cycle();
	return 0;
}

void eat_and_think_cycle()
{
//setup a signal handler to stop a philisopher process
	signal(15, signalHandler);
	do
	{
		sem_wait(&chopstick[phil]);
		sem_wait(&chopstick[(phil+1) % seats]);
		eat();
		sem_post(&chopstick[phil]);
		sem_post(&chopstick[(phil+1) % seats]);
		cycles++;
		thinking();
	}while(1);

}

void signalHandler(int mysignal)
{
	if(mysignal == 15)
	{
		sem_close(&chopstick[phil]);
		sem_close(&chopstick[(phil+1)%seats]);
	}
	fprintf(stderr, "Philosopher%d completed %d \n", phil, cycles);

}

void eat()
{
	srand(time(0));
	int random = rand();
	usleep(random);
	printf("Philosopher %d is eating\n", phil);
}

void thinking()
{
	srand(time(0));
	int random = rand();
	usleep(random);
	printf("Philosopher %d is thinking\n", phil);
}