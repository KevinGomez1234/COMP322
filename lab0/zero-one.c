#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void logic(int argc, char* argv []);
void printInformation(int file_descriptor, int terminator);
void printArgsInformation(int argc, char* argv[]);

//Non-printable ascii characters 0-32 && 127
static const char nonPrintables [34][6] = 
{
	"NUL  ", "SOH  ", "STX  ", "ETX  ", "EOT  ", "ENQ  ", "ACK  ", "BEL  ", 
	"BS   ", "HT   ", "LF   ", "VT   ", "FF   ", "CR   ", "SO   ", "SI   ", 
	"DLE  ", "DC1  ", "DC2  ", "DC3  ", "DC4  ", "NAK  ", "SYN  ", "ETB  ",
	"CAN  ", "EM   ", "SUB  ", "ESC  ", "FS   ", "GS   ", "RS   ", "US   ","SPACE","DEL  "
};


int main(int argc,char* argv [])
{
	logic(argc,argv);
	return 0;
}

//Prints out information if we're reading from a file.
void printInformation(int file_descriptor, int terminator)
{
	int decimal = 0;
	int counter = 7;
	int parityChecker = 0;
	char ch[1];
	static const char* EVEN = "EVEN";
	static const char* ODD = "ODD";
	const char** ptr;
	printf("Original        ASCII        Decimal        Parity \n");
	printf("-------        -------        -------        ------- \n");
	//Read from a file using read()
	while(read(file_descriptor, &ch[0], sizeof(ch)) > 0 && ch[0] != terminator)
	{
		//Only consider 1's or 0's for the input and and don't run this if we've read all 8 characters
		if(((int)ch[0] == 49 ||(int)ch[0] == 48) && counter != -1 )
		{	
			//atoi() converts a string to an integer easily or we could do (int)ch[0]%48, but atoi() is more readable.  
			printf("%d", atoi(&ch[0]));
			if (atoi(&ch[0]) == 1)
				parityChecker++;
			if(counter!=7)
				//atoi returns the actual value of ch[0] which is either a 1 or a 0.
				decimal = (pow(2, counter) * atoi(&ch[0])) + decimal;
			counter--;
		}//endwhile

		//we've read 8 chars therefore print the missing information out
		if (counter == -1)
		{
			//check parity of binary string and point to correct string (odd or even).
			if(parityChecker % 2 == 0)
				ptr = &EVEN;
			else
				ptr = &ODD;
			//edge cases to print out 0-32 or 127
			if(decimal>=0 && decimal<=32)
				printf("\t  %s         %d\t\t%s\n", nonPrintables[decimal],decimal, *ptr);
			else if(decimal == 127)
				printf("\t  %s         %d\t\t%s\n", nonPrintables[33],decimal, *ptr);
			else	
				printf("\t  %c             %d\t\t%s\n", decimal,decimal, *ptr);
			//reset vars
			counter = 7;
			decimal = 0;
			parityChecker = 0;
		}
	}

	//if counter has not ended and we havent started reading new chars, but we reached end of the loop, then we have extra 0's to pad.
	if(counter != -1 && counter != 7)
	{
		if(parityChecker % 2 == 0)
			ptr = &EVEN;
		else
			ptr = &ODD;
		for(int i = 0;i < counter + 1;i++)
		{
			printf("0");
		}
			//edge cases
		if(decimal>=0 && decimal<=32)
			printf("\t  %s         %d\t\t%s\n", nonPrintables[decimal],decimal, *ptr);
		else if(decimal == 127)
			printf("\t  %s         %d\t\t%s\n", nonPrintables[33],decimal, *ptr);
		else	
			printf("\t  %c             %d\t\t%s\n", decimal,decimal, *ptr);
		counter = 7;
		decimal = 0;
		parityChecker = 0;
	}
}

//reading the args with a for loop. 
void printArgsInformation(int argc, char* argv[])
{
	int arraySize;
	int decimal = 0;
	int counter = 7;
	int parityChecker = 0;
	const char** ptr;
	static const char* EVEN = "EVEN";
	static const char* ODD = "ODD";
	char ch[1];
	printf("Original        ASCII        Decimal        Parity \n");
	printf("-------        -------        -------        ------- \n");
	//read argv[] starting from 1. 
	for(int i = 1; i<argc;i++)
	{
		//the length of the array is strlen or until argv[i][n] == \0. 
		arraySize = strlen(argv[i]); 
		for(int j = 0; j<arraySize;j++)
		{
			if(argv[i][j] == (int)'1' || argv[i][j] == (int)'0')
			{
				ch[0] = argv[i][j];
				printf("%d", atoi(&ch[0]));
				if(atoi(&ch[0]) == 1)
					parityChecker++;
				if(counter != 7)
					//atoi returns the actual value of ch[0] which is either a 1 or a 0.
					decimal = (pow(2, counter) * atoi(&ch[0])) + decimal;
				counter--;
			}

			if(counter == -1)
			{
				if(parityChecker % 2 == 0)
					ptr = &EVEN;
				else
					ptr = &ODD;
				//edge cases
				if(decimal>=0 && decimal<=32)
					printf("\t  %s         %d\t\t%s\n", nonPrintables[decimal],decimal, *ptr);
				else if(decimal == 127)
					printf("\t  %s         %d\t\t%s\n", nonPrintables[33],decimal, *ptr);
				else	
					printf("\t  %c             %d\t\t%s\n", decimal,decimal, *ptr);
				counter = 7;
				decimal = 0;
				parityChecker = 0;
			}
		}
	}

	// pad extra 0's
	if(counter != -1 && counter != 7)
	{
		if(parityChecker % 2 == 0)
			ptr = &EVEN;
		else
			ptr = &ODD;
			//print out missing 0's
		for(int i = 0;i < counter + 1;i++)
		{
			printf("0");
		}
		//edge cases 
		if(decimal>=0 && decimal<=32)
			printf("\t  %s         %d\t\t%s\n", nonPrintables[decimal],decimal, *ptr);
		else if(decimal == 127)
			printf("\t  %s         %d\t\t%s\n", nonPrintables[33],decimal, *ptr);
		else	
			printf("\t  %c             %d\t\t%s\n", decimal,decimal, *ptr);
		counter = 7;
		decimal = 0;
		parityChecker = 0;
	}
}

//Logic for the program
void logic (int argc,char* argv [])
{
	//if no input is passed there is an error
	if(argc == 1)
		printf("Invalid input program will now exit.\n");
	else if(argc >= 2)
	{
		//assume input is a file
		int fd = open(argv[1], O_RDONLY);
		//if not valid file, then check if it's a stream of 1's and 0's
		if(fd == -1)
		{
			close(fd);
			//if the second argument doesnt begin with 1 or 0 or - then it's probably a mistake...
			if(*argv[1] == '-' || argv[1][0] == '1' || argv[1][0] == '0')
				printArgsInformation(argc, argv);
			//else it's not valid input
			else 
				printf("Invalid input, program will now exit\n");
		}
		//if it is valid file then open and read it with printInformation();
		else
		{
			int fd = open(argv[1], O_RDONLY);
			printInformation(fd, -1);
		}
	}
}