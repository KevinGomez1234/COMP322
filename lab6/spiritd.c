#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <syslog.h>
#include <limits.h> //used for getting the path to C file 
#include <string.h>

void sig_handler();
void createMole();

int mole1;
int mole2;
int main()
{
	pid_t parent_pid;
	struct rlimit rLimitStruct;
	int dev_null_fd, log_fd;
	char cwd [PATH_MAX];
	//daemon should register 3 signals not child: USR1, USR2, and TERM
	signal(SIGUSR1, sig_handler);
	signal(SIGUSR2,sig_handler);
	signal(SIGTERM, sig_handler);

	//fork a process
	parent_pid = fork();
	//Fork a processs and terminate the parent
	if(parent_pid > 0)
	{
		exit(0);
	}
	//any files/directories created  (owner,groups, and other) have permission for read/write.
	umask(0);
	//child, create a new session with setsid(2)
	setsid();	
	//get the path to the current directory before we switch to root. Used to open the .log file in our project's directory. Alternative use system log (syslog)
	getcwd(cwd, sizeof(cwd));
	strcat(cwd, "/lab6.log");
	printf("cwd: %s\n", cwd);
	//open the file and make it available to others, owner, and group rwe permissions.
	log_fd = open(cwd, O_CREAT, 00777);
	if(log_fd == -1)
		printf("This is an error\n");
	//change directory to root
	chdir("/");
	//get the maximum number of open files for this process
	getrlimit(RLIMIT_NOFILE, &rLimitStruct);
	//open a log file
	if(rLimitStruct.rlim_max == RLIM_INFINITY)
		rLimitStruct.rlim_max = 1024;
	for(unsigned int i = 0; i<rLimitStruct.rlim_max;i++)
		close(i);
	dev_null_fd = open("/dev/null", O_RDWR);
	dup2(dev_null_fd, 0);
	dup2(dev_null_fd, 1);
	dup2(dev_null_fd, 2);
	//while(1)
	//{
	//	pause();
	//}
	return 0;
}

//
void sig_handler(int signal)
{
//if mole1 or mole 2 != then it is the current process, so kill it. and then finally end the current process which is the deamon.
	if(signal == SIGTERM)
	{
		if(mole1 != 0){
			kill(mole1, SIGTERM);
			mole1 = 0;
		}
		if(mole2 != 0){
			kill(mole2, SIGTERM);
			mole2 = 0;
		}
		exit(0);
	}

//kill mole 1 and fork another random mole need to execve mole program and find the path to run it.
	else if(signal == SIGUSR1)
	{
		if(mole1 != 0)
		{
			kill(mole1, SIGTERM);
			mole1 = 0;
		}
		createMole();
	}
//kill mole 2 and fork another random mole
	else if(signal == SIGUSR2)
	{
		if(mole2 != 0)
		{
			kill(mole2, SIGTERM);
			mole2 = 0;
		}
		createMole();
	}
}


void createMole()
{
	srand(time(0));
	char c[100];
	char* path;
	int random = (rand()%2) + 1;
	char* args[3];
	char* mole_num;

	path = getcwd(c, 100);
	strcat(path, "/mole");

	if(random == 1)
		mole_num = "mole 1";
	else
		mole_num = "mole 2";
	args[0] = path;
	args[1] = mole_num;
	args[2] = NULL;
	execv(args[0], args);
}

