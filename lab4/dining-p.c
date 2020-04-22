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


void eat();
void thinking();
void eat_and_think_cycle();
void initializeSemapores();
void signalHandler();
int phil;
int seats;
int cycles;

//array of names of semaphores, need to find a
char* names[5] = {"a0 ","a1 ", "a2 ", "a3 ", "a4 "};
//semaphor for each chopstick
sem_t* chopsticks[5]; 
int seats;
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
	chopsticks [phil] = sem_open(names[phil], O_CREAT|O_EXCL, 0666, 1);
	chopsticks [(phil+1)%seats] = sem_open(names[(phil+1)%seats], O_CREAT|O_EXCL, 0666);
	if(chopsticks[phil-1] == SEM_FAILED)
		perror("sem_open failed");
	if(chopsticks [phil%seats] == SEM_FAILED)
		perror("sem_open failed");
}

void eat_and_think_cycle()
{
//setup a signal handler to catch SIGTERM and display cycles
	signal(15, signalHandler);
//open up our semaphores to be used with
	do
	{
		sem_wait(chopsticks[phil]);
		sem_wait(chopsticks[(phil+1) % seats]);
		eat();
		sem_post(chopsticks[phil]);
		sem_post(chopsticks[(phil+1) % seats]);
		cycles++;
		thinking();
	}while(1);

}

void signalHandler(int mysignal)
{
	if(mysignal == 15)
	{
		sem_close(chopsticks[phil]);
		sem_close(chopsticks[(phil+1)%seats]);
		sem_unlink(names[phil]);
		sem_unlink(names[(phil+1)%seats]);
		sem_destroy(chopsticks[phil]);
		sem_destroy(chopsticks[(phil+1)%seats]);
	}
	fprintf(stderr, "Philosopher%d completed %d \n", phil, cycles);
}

//done
void eat()
{
	srand(time(0));
	int random = rand() % (1000000 + 1 - 0) + 0;
	usleep(random);
	printf("Philosopher %d is eating pid %d\n", phil, getpid());
}

void thinking()
{
	srand(time(0));
	int random = rand() % (1000000 + 1 - 0) + 0;
	usleep(random);
	printf("Philosopher %d is thinking pid %d\n", phil, getpid());
}