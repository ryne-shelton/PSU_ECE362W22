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
 * Description:  
 *
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

void err_sys(char *msg)
{
	fprintf(stderr,"%s (%s)\n",msg,strerror(errno));
	exit(-1);
}

int main(int argc, char *argv[]){
	time_t start, end, execute_time;
	char user_input[50];
	for (int i=0; i<argc-1; i++){
		strcat(user_input," ");
		strcat(user_input,*++argv);
	}
	printf("input: %s\n", user_input);
	if(write(STDIN_FILENO,user_input,strlen(user_input)) != strlen(user_input)){
			err_sys("write to STDOUT error");
		}
	start = time(NULL);
	system(user_input);
	end = time(NULL);
	execute_time = end - start;
	printf("\nSeconds to execute: %ld\n", execute_time);
	return 0;
}