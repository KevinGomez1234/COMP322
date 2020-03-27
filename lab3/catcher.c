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
static const char signalNumbers [31] [11] = {"HUP", "INT", "QUIT", "ILL", "TRAP", "ABRT", "BUS", "FPE", "KILL", "USR1", "SEGV", "USR2", "PIPE", "ALRM", "TERM", "STKFLT", "CHLD", "CONT", "STOP", "TSTP", "TTIN", "TTOU", "URG", "XCPU", "XFSZ", "VTALRM", "PROF", "WINCH", "IO", "PWR", "SYS"};
//global variables are needed since handler can't have parameters, term_counter counts the number of times TERM is executed within the handler.
int term_counter = 0;

int main(int argc, char* argv[])
{
	logic(argc, argv);
	return 0;
}

//handler for all our signals
void myhandle(int mysignal)
{
	int signaleName = returnSigIndex(mysignal);
	if(mysignal == 15)
		term_counter++;
	fprintf(stdout, "SIG%s caught at %li \n", signalNumbers[signaleName], time(NULL));
	//register signal again
	signal(mysignal, myhandle);
}

//logic for the program also checks for errors
void logic(int argc, char* argv[])
{
	int signals_caught = 0;
	// no args means no signals to catch, so it's an error.
	if(argc < 2)
		printf("Improper usage(No signals in arguments): ./catcher <signal>... <signal>\n");
	//check the args for proper signals
	else {
		fprintf(stderr, "catcher: $$ = %d \n",getpid());	
		//register all the proper handlers
		for(int i = 1; i<argc; i++) {
			int sigNumber = returnSigNumber(argv[i]);
			if(sigNumber != -1)
				signal(sigNumber, myhandle);
			//the argument is not a signal then don't do anything
			else
				continue;
		}
		//wait for signals
		while(1) {
			//wait for handle to finish, count it as a signal caught
			if (pause())
				signals_caught++;
			//after 3 successive term signals, stop
			if(term_counter == 4)
				break;
		}
		fprintf(stderr, "catcher: Total signals caught = %d\n", signals_caught);
	}
}

//Returns the corresponding signal number for a string i.e TERM = 15, returns -1 on sigstop and sigkill
int returnSigNumber(char string[])
{
	//if we retunr -1 the signal does not exist in our map. 
	int sigNumber = -1;
	//had to use an array to map signals b/c sys_signame only works on os x, and also sys_siglist[] only has decritpions for signals and does not their mneumonic signal name i.e sys_siglist[1] = "HANHUP" and not "SIGHUP"
	for(int i = 0; i<31;i++) {
		//Had to use strcmp to map the proper command line args to our signalNumbers array.
		if(!strcmp(string, signalNumbers[i])) {
			sigNumber = i + 1;
			break;			
		}
		//special case for IOT = 6
		else if(!strcmp(string, "IOT")) {
			if(!strcmp(string, "IOT"))
				sigNumber = SIGIOT;
		}
		//ignore kill and stop
		else if(!strcmp(string, "KILL") | !strcmp(string, "STOP")) {
			sigNumber = -1;
		}
	}
	return sigNumber;
}

//returns the proper index in our array containing the names of the signals i.e returnSigIndex(1) = signalNumbers[0] = "HUP"
int returnSigIndex(int signalNumber)
{
	int arrayIndex;
	if(signalNumber >= 1 && signalNumber <= 31 ) {
		arrayIndex = signalNumber - 1;
	}
	return arrayIndex;
}