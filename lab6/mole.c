#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h> //used for getting the path to C file 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
	//
	FILE* fp;
	(void) argc;
	char path_to_home [PATH_MAX];
	//we will be passing the path to the lab6.log file through argv[2]
	chdir("/");
	strcat(path_to_home, getenv("HOME"));
	strcat(path_to_home, "/lab6.log");
	printf("path_to_home: %s\n", path_to_home);
	fp = fopen(path_to_home, "a");
	if(fp == NULL)
		printf("lab6.log has not been initialized...\n");
	fprintf(fp, "Pop mole%s\n", argv[1]);
	fclose(fp);
	while(1){
		//wait for calling process to terminate ...
	}
	return 0;
}