#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void logic();
void myhandle();
int returnSigNumber();
int returnSigIndex();
//[31] [32] special case signals (duplicate signal numbers)
static const char signalNumbers [33] [11] = {"HUP", "INT", "QUIT", "ILL", "TRAP", "ABRT", "BUS", "FPE", "KILL", "USR1", "SEGV", "USR2", "PIPE", "ALRM", "TERM", "STKFLT", "CHLD", "CONT", "STOP", "TSTP", "TTIN", "TTOU", "URG", "XCPU", "XFSZ", "VTALRM", "PROF", "WINCH", "IO", "PWR", "SYS", "IOT", "POLL"};
int counter = 0;

int main(int argc, char* argv[])
{
	logic(argc, argv);
	return 0;
}

void myhandle(int mysignal)
{
	int signaleName = returnSigIndex(mysignal);
	if(mysignal == 15)
		counter++;
	fprintf(stderr, "SIG%s caught at %li \n", signalNumbers[signaleName], time(NULL));
	signal(mysignal, myhandle);
}

//logic for the program
void logic(int argc, char* argv[])
{
	int signals_caught = 0;
	// no args then it's an error
	if(argc < 2)
		printf("Improper usage(No signals in arguments): ./catcher <signal>... <signal>\n");
	//check the args for proper signals
	else {
		fprintf(stderr, "%s: $$ = %d \n", argv[0],getpid());	
		for(int i = 1; i<argc; i++) {
			int sigNumber = returnSigNumber(argv[i]);
			if(sigNumber != -1)
				signal(sigNumber, myhandle);
			//the argument is not a signal then don't do anything
			else
				continue;
		}

		while(1) {
			//wait for handle to finish
			if (pause())
				signals_caught++;
			if(counter == 3)
				break;
		}
		printf("Signals caught: %d\n", signals_caught);
	}
}

//Returns the corresponding signal number for a string i.e TERM = 15
int returnSigNumber(char string[])
{
	//if we retunr -1 the signal does not exist in our map. 
	int sigNumber = -1;
	//had to use an array to map signals b/c sys_signame only works on os x, and also sys_siglist[] only has decritpions for signals and does not their mneumonic signal name i.e sys_siglist[1] = "HANHUP" and not "SIGHUP"
	for(int i = 0; i<31;i++) {
		if(!strcmp(string, signalNumbers[i])) {
			sigNumber = i + 1;
			break;			
		}
		//special cases IOT = 6, POLL = 29, UNUSED = 31
		else if(!strcmp(string, "IOT")| !strcmp(string, "POLL")) {
			if(!strcmp(string, "IOT"))
				sigNumber = SIGIOT;
			else if(!strcmp(string, "POLL"))
				sigNumber = SIGPOLL;
		}
	}
	return sigNumber;
}

//returns the proper index in our array containing the names of the signals
int returnSigIndex(int signalNumber)
{
	int arrayIndex;
	if(signalNumber >= 1 && signalNumber <= 31 ) {
		arrayIndex = signalNumber - 1;
	}
	else if(signalNumber == SIGIO) {
		arrayIndex = 31;
	}
	else if(signalNumber == SIGPOLL) {
		arrayIndex = 32;
	}
	return arrayIndex;
}

