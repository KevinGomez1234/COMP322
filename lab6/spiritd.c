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
#include <sys/wait.h>

void sig_handler();
void createMole();
//used to distinguish mole1 and mole2 (USR1 and USR2)

pid_t mole1_pid;
pid_t mole2_pid;
//path to the mole program
char path_to_mole [PATH_MAX];
int main()
{
	pid_t parent_pid;
	struct rlimit rLimitStruct;
	int dev_null_fd, log_fd;
	char path_to_home [PATH_MAX];
	//fork a process
	parent_pid = fork();
	//Fork a processs and terminate the parent
	if(parent_pid > 0)
	{
		exit(0);
	}
	//daemon process
	else if(parent_pid == 0)
	{
		//Start of Daemon
		//set umask(0) for rwe files
		umask(0);
		//print pid daemon and catch signals
		signal(SIGUSR1, sig_handler);
		signal(SIGUSR2,sig_handler);
		signal(SIGTERM, sig_handler);
		//create a new session with setsid(2)
		setsid();	
		//Get the path to mole 
		getcwd(path_to_mole, sizeof(path_to_mole));
		strcat(path_to_mole, "/mole");
		//change directory to root
		chdir("/");
		//open a file for logging
		strcat(path_to_home, getenv("HOME"));
		strcat(path_to_home, "/lab6.log");
		log_fd = open(path_to_home, O_CREAT, 0777);
		if(log_fd == -1)
			perror("open() error ");
		//get the maximum number of open files for this process, and close them all
		getrlimit(RLIMIT_NOFILE, &rLimitStruct);
		if(rLimitStruct.rlim_max == RLIM_INFINITY)
			rLimitStruct.rlim_max = 1024;
		for(unsigned int i = 0; i<rLimitStruct.rlim_max;i++)
			close(i);	
		//reopen stdin,stdout, and stderror and map them to 
		dev_null_fd = open("/dev/null", O_RDWR);
		dup2(dev_null_fd, 0);
		dup2(dev_null_fd, 1);
		dup2(dev_null_fd, 2);
		while(1)
		{
			pause();
		}
	}
	return 0;
}

void sig_handler(int sig)
{
	//register signal again.
	signal(sig, sig_handler);

	//Kill mole1 or mole2 if they exist and terminate the daemon.
	if(sig == SIGTERM)
	{
		int mole1_status;
		int mole2_status;
		//check if the process exists, if process exists
		if(waitpid(mole1_pid, &mole1_status, WNOHANG) == 0)
		{
			kill(mole1_pid, SIGKILL);
		}

		if(waitpid(mole2_pid, &mole2_status, WNOHANG) == 0)
		{
			kill(mole2_pid, SIGKILL);
		}

		//exit out of Deamon process
		kill(getpid(), SIGKILL);
	}
	//kill mole1 process and fork another process.
	else if(sig == SIGUSR1)
	{
		int mole1_status;
		//If mole 1 exists then kill it. Create a new mole afterwards... 
		if(waitpid(mole1_pid, &mole1_status, WNOHANG) == 0)
		{
			kill(mole1_pid, SIGKILL);
		}
		//if we called SIGUSR1 and mole2 is processing, then don't do anything because user made a mistake "wacking" the wrong mole. This prevents us from creating more than 3 processes.... 
		else if(waitpid(mole2_pid, &mole1_status, WNOHANG) == 0)
		{
			return;
		}
		//create mole
		createMole();
	}
	//kill mole2 if it exists and fork another process.
	else if(sig == SIGUSR2)
	{
		int mole2_status;
		//If mole 1 exists then kill it. Create a new mole afterwards... 
		if(waitpid(mole2_pid, &mole2_status, WNOHANG) == 0)
		{
			kill(mole2_pid, SIGKILL);
		}
		//if we called SIGUSR2 and mole1 is processing, then don't do anything because user made a mistake. This prevents us from creating more than 3 processes....
		else if(waitpid(mole1_pid, &mole2_status, WNOHANG) == 0)
		{
			return;
		}
		//create mole
		createMole();
	}
}

//create mole1 or mole2 process. 
void createMole()
{
	srand(time(0));
	int random = (rand()%2);
	char* args[3];
	char* mole_num;
	if(random == 1)
	{
		mole1_pid = fork();
		mole_num = "1";
		//child of daemon will execute mole1
		if(mole1_pid == 0)
		{
			args[0] = path_to_mole;
			args[1] = mole_num;
			args[2] = NULL;
			execv(args[0], args);
		}
	}
	else
	{
		mole2_pid = fork();
		mole_num = "2";
		//child of daemon will execute mole1
		if(mole2_pid == 0)
		{
			args[0] = path_to_mole;
			args[1] = mole_num;
			args[2] = NULL;
			execv(args[0], args);
		}

	}
}