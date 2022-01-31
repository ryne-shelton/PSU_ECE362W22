/*
 * piper.c - Source code for for piper program
 *
 * @authors 
 *          Ibrahim Binmahfood (ibrah5@pdx.edu)
 *          Ryne Shelton       (ryn2@pdx.edu)
 *
 * @date 24-Jan-2022
 * 
 * (ECE 362 Winter 2022 HW #2)
 *
 * Description:	This program takes input from stdin with system calls, gives it to two 
 *				child processes seperately they calculate the sum and products of the
 *				entered input and writes their respective results to stdout. 
 *
 */
//=============================================================================================
// Libraries 
//=============================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
//=============================================================================================
// Macros
//=============================================================================================
#define BUFFSIZE 	4096
//=============================================================================================
// Helper Functions 
//=============================================================================================
void err_sys(char *msg) 	// writes error message and returns 1
{
	fprintf(stderr,"%s (%s)\n",msg,strerror(errno));
	exit(-1);
}
//=============================================================================================
// Main Program
//=============================================================================================
int main()
{
	// variable declarations
	char	ch, buff[BUFFSIZE];  		// data buffer for passing things
	// fd[0] - parent/child0 pipe, fd[1] - parent/child1 pipe
	int 	fd[2][2], nread, i=0,n=0; 	// fd[][0] - read , fd[][1] - write
	char* 	user_input[50];
//=============================================================================================
// 
//=============================================================================================

	// reads input into buffer "buff" until ctrl-d 
	while((nread=read(STDIN_FILENO, &ch, 1)) > 0){
		buff[n]=ch;
		n++;
	}
	if(nread<0) err_sys("read error");

//=============================================================================================	
// Creates Pipes
//=============================================================================================
	for(int k=0;k<2;k++){
		if(pipe(fd[k])<0) err_sys("\npipe error");	// open pipes
	}
//=============================================================================================
// Creates Child0 that recieves buff from parent then summs the values and prints out the 
// sum. 
//=============================================================================================
	int child0 = fork();	// Creates child0 (addition)
	if(child0<0) err_sys("\nfork error");	
	if(child0==0){			// Child process 0	
		// close unused pipes
		close(fd[0][1]);	// close write end at parent to child0
		close(fd[1][0]);	// close read end at child1 from parent
		close(fd[1][1]);	// close write end at parent to child1
		if(read(fd[0][0],buff, BUFFSIZE)<0){ // read pipe from parent
			err_sys("\nread error");
		}
		close(fd[0][0]);				// close read end at child0 from parent
		for (char *token = strtok(buff," \n"); token != NULL; token = strtok(NULL, " \n")) {
			user_input[i] = token; 
			i++;
		}
//?????????????????????????????????????????????????????????????????????????????????????????????
		// DEBUGGER CODE
		printf("\n\n");
		for(int j=0; j<i;j++){
			printf("child 0 user_input[%d]: %s\n",j,user_input[j]);
		}
		printf("\nstdout0:\n");
		write(STDOUT_FILENO,buff,BUFFSIZE);
//?????????????????????????????????????????????????????????????????????????????????????????????

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		//add code here to perform addition on user_input 
		
		
		
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		return 0;
	}	
//=============================================================================================
// Creates Child1 that recieves buff from parent then multiples the values and prints out the 
// product. 
//=============================================================================================
	int child1 = fork();	// Creates child1 (multiplication)
	if(child1<0) err_sys("\nfork error");	
	if(child1==0){			// Child process 1
		// close unused pipes
		close(fd[0][0]);	// close read at child0 from parent
		close(fd[0][1]);	// close write at parent to child0
		close(fd[1][1]);	// close write at parent to child1
		if(read(fd[1][0],buff, BUFFSIZE)<0){ // read pipe from parent
			err_sys("\nread error");
		}
		close(fd[1][0]);				// close read at child 1 from parent 
		for (char *token = strtok(buff," \n"); token != NULL; token = strtok(NULL, " \n")) {
			user_input[i] = token; 
			i++;
		} 
//?????????????????????????????????????????????????????????????????????????????????????????????
		// DEBUGGER CODE
		printf("\n\n");
		for(int j=0; j<i;j++){
			printf("child 1 user_input[%d]: %s\n",j,user_input[j]);
		}
//?????????????????????????????????????????????????????????????????????????????????????????????

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		// add code here to perfrom multiplication 
		
		
		
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		return 0;
	}
//=============================================================================================
// Parent process that writes buff to both of its children 
//=============================================================================================
	// Parent process
	close(fd[0][0]);	// close read at child0 from parent
	close(fd[1][0]);	// close read at child 1 from parent 
	if(write(fd[0][1],buff,BUFFSIZE)<0){
		err_sys("\nwrite error");
	}
	close(fd[0][1]);	// close write at parent to child0
	if(write(fd[1][1],buff,BUFFSIZE)<0){
		err_sys("\nwrite error");
	}
	close(fd[1][1]);	// close write at parent to child1
	printf("\nstdoutfinal:\n");
	return 0; 
}

