/*
 * timer.c - Source code for for timer program
 *
 * @authors 
 *          Ibrahim Binmahfood (ibrah5@pdx.edu)
 *          Ryne Shelton       (ryn2@pdx.edu)
 *
 * @date 24-Jan-2022
 *
 * (ECE 362 Winter 2022 HW #2) 
 *
 * Description: This program takes a specified command from the user as arguements,
 * 				executes the command, and prints out the commands execution time 
 *
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void err_sys(char *msg) // error handler
{
	fprintf(stderr,"%s (%s)\n",msg,strerror(errno));
	exit(-1);
}
int main(int argc, char *argv[]){
	int start, end, execute_time; // time variables
	char user_input[50],result[20],final[50]="\nexecute time: "; // string for command input
	for (int i=0; i<argc-1; i++){
		if (i>0) strcat(user_input," "); // adds space between arguements 
		strcat(user_input,*++argv); // adds arguements to string
	}
	start = (int)time(NULL); // time of start
	system(user_input); // executes specified command
	end = (int)time(NULL); // time of end
	execute_time = end - start; // difference = total time
	snprintf(result,10,"%d",execute_time); // converts to string to print
	strcat(final,strcat(result,"\n")); // adds newline character to result and concats to final
	if(write(STDOUT_FILENO,final,strlen(final)) != strlen(final)){ // writes final string to stdout
			err_sys("write to STDOUT error"); // calls errror if write fails
		}
	return 0;
}