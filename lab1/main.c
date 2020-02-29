#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/wait.h>

void printProcessInformation();
void logic();
//gets new variables after executing a new process
int main(void)
{
	logic();
}

void logic()
{
	long time_start, time_end;
	int status = 0;
	//tms has all of the variables we need to report process times, returns tics 
	struct tms t;
	int ticsPerSecond = sysconf(_SC_CLK_TCK);
	//start of the program 
	time_start = time(NULL); 
	printf("START: %li\n", time_start);
	//spawn a process
	pid_t pid = fork();
	//we are the child process, print information
	if((int)pid == 0)
	{
		printProcessInformation((int)pid, status);
		exit(0);
	}
	
	//we are the parent process so wait for the child
	else if((int)(pid) > 0)
	{	
		waitpid(pid, &status, 0);
		//times() calculates all user, sys, utime children, and sys time children.
		times(&t);
		printProcessInformation((int)pid, status);
		printf("USER: %f, SYS: %f \n", (double)t.tms_utime / (double)ticsPerSecond, (double)t.tms_stime);
		printf("CUSER: %f, CSYS: %f \n", (double)t.tms_cutime / (double)ticsPerSecond, (double)t.tms_cstime / (double)ticsPerSecond);
	}
	//end time 
	time_end = time(NULL); 
	printf("STOP: %li\n", time_end);
}

void printProcessInformation(int pid, int status)
{
	//we are the parent print the exit status and cpid
	if (pid != 0)
	{
		int returned = WEXITSTATUS(status);
		printf("PPID: %d, PID: %d, CPID: %d, RETVAL: %d \n", getppid(), getpid(), pid, returned);
	}
	//we are child, print ppid and pid
	else
		printf("PPID: %d, PID: %d \n", getppid(), getpid());
}