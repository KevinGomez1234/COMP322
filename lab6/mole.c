#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <string.h>

void sig_handler();
void createMole();

int mole1;
int mole2;
int main()
{
pid_t parent_pid;
pid_t new_session;
struct rlimit rLimitStruct;
int fd0, fd1, fd2;

//daemon should register 3 signals not child: USR1, USR2, and TERM
signal(SIGUSR1, sig_handler);
signal(SIGUSR2,sig_handler);
signal(SIGTERM, sig_handler);

  //*** clear log file before the mole children write to it  ***//

char *file_path;

file_path = (char*) malloc(sizeof(getenv("HOME")));   // allocate memory for size of char* of <home path>
strcpy(file_path, getenv("HOME"));

file_path = (char*) realloc(file_path, sizeof(getenv("HOME")) + 10);  // extra 10 spots for static filename
strcat(file_path, "/lab6.log");   // concatenate filename to path


FILE *logfile = fopen(file_path, "w+"); // absolute path for <home directory>/lab6.log
if (logfile == NULL)
{
   printf("Cannot find file: lab6.log\n");
   exit(1);
}

fclose(logfile);

free(file_path);

parent_pid = fork();
//any files/directories created  (owner,groups, and other) have permission for read/write.
umask(0);
//Fork a processs and terminate the parent
if(parent_pid > 0)
{
exit(0);
}
//child, create a new session with setsid(2)
new_session = setsid();
//change directory to root
chdir("/");
//get the maximum number of open files for this process
getrlimit(RLIMIT_NOFILE, &rLimitStruct);
//close them all
if(rLimitStruct.rlim_max == RLIM_INFINITY)
{
rLimitStruct.rlim_max = 1024;
}

for(int i = 0; i<rLimitStruct.rlim_max;i++)
{
close(i);
}
//only reopoen std_out, std_in, and stderror and redirect to /dev/null
fd0 = open("/dev/null", O_RDWR);
fd1 = dup(0);
fd2 = dup(0);

while(1){
pause();
}
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


void createMole(int mole)
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
