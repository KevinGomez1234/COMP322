#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 4096

void logic();
int calculatePageNumber();
int calculateOffset();
int main(int argc, char* argv[])
{
	//4kb = 4096 bytes
	logic(argc, argv);
}

void logic(int argc, char* argv[])
{
	unsigned int pageNumber;
	unsigned int offset;
	unsigned int address = atoi(argv[1]);
	if (argc < 2)
	{
		printf("Improper usage: ./paging <int>\n");
	}
	else
	{
		pageNumber = calculatePageNumber(address);
		offset = calculateOffset(address);
		printf("The address %d contains:\nPage number = %d \nOffset = %d\n", address, pageNumber, offset);
	}
}

int calculatePageNumber(unsigned int address)
{
	return address/PAGE_SIZE;
}

int calculateOffset(unsigned int address)
{
	return address % PAGE_SIZE;
}