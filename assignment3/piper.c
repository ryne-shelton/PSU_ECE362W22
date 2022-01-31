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
 * Description:  
 *
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
// Helper Function 
//=============================================================================================
void err_sys(char *msg)
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
	char	buff0[BUFFSIZE],buff1[BUFFSIZE];  // two data buffers for passing things
	// fd[0] - parent/child0 pipe, fd[1] - parent/child1 pipe
	int 	i, fd[2][2],read_fd, write_fd, nread; // fd[][0] - read , fd[][1] - write
	char* user_input[50];
	char* filename = "user_input";
//=============================================================================================
// Takes user input from stdin and writes it to a temporary file one line at a time, reads in
// the temporary file once user singals that they are finished entering input (ctrl-d) to a 
// buffer (buff1) and then deletes the temporary file (user_input).
//=============================================================================================
	if((write_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, S_IRWXU)) < 0){
		err_sys("open error");
	}
	// read input into buffer until ctrl-d ** needs work 
	while((nread=read(STDIN_FILENO, buff0, BUFFSIZE)) > 0){
		// write to file
		if(write(write_fd,buff0,nread) != nread){
			err_sys("write to file error");
		}
	}
	if(nread<0) err_sys("read error");
	close(write_fd); 			// closes temporary file
	// Opens file in read only mode
	if((read_fd = open(filename, O_RDONLY)) < 0){
		err_sys("open error");
	}
	// reads temp file into buff1
	if((nread = read(read_fd, buff1,BUFFSIZE)) < 0 ){
		err_sys("read error"); 	// error if read fails
	}
	close(read_fd); 			// closes temporary file
	system("rm user_input"); 	// deletes temporary file
//=============================================================================================	
// Creates Pipes
//=============================================================================================
	for(int i=0;i<2;i++){
		if(pipe(fd[i])<0) err_sys("\npipe error");	// open pipes
	}
//=============================================================================================
// Creates Child0 that recieves buff1 from parent then summs the values and prints out the 
// sum. 
//=============================================================================================
	int child0 = fork();	// Creates child0 (addition)
	if(child0<0) err_sys("\nfork error");	
	if(child0==0){			// Child process 0	
		// close unused pipes
		close(fd[0][1]);	// close write end at parent to child0
		close(fd[1][0]);	// close read end at child1 from parent
		close(fd[1][1]);	// close write end at parent to child1
		if(read(fd[0][0],buff1, BUFFSIZE)<0){ // read pipe from parent
			err_sys("\nread error");
		}
		close(fd[0][0]);				// close read end at child0 from parent
		for (char *token = strtok(buff1," \n"); token != NULL; token = strtok(NULL, " \n")) {
			user_input[i] = token; 
			i++;
		}
//?????????????????????????????????????????????????????????????????????????????????????????????
		// DEBUGGER CODE
		printf("\n\n");
		for(int j=0; j<i;j++){
			printf("child 0 user_input[%d]: %s\n",j,user_input[j]);
		}
//?????????????????????????????????????????????????????????????????????????????????????????????

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		//add code here to perform addition on user_input 
		
		
		
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		return 0;
	}	
//=============================================================================================
// Creates Child1 that recieves buff1 from parent then multiples the values and prints out the 
// product. 
//=============================================================================================
	int child1 = fork();	// Creates child1 (multiplication)
	if(child1<0) err_sys("\nfork error");	
	if(child1==0){			// Child process 1
		// close unused pipes
		close(fd[0][0]);	// close read at child0 from parent
		close(fd[0][1]);	// close write at parent to child0
		close(fd[1][1]);	// close write at parent to child1
		if(read(fd[1][0],buff1, BUFFSIZE)<0){ // read pipe from parent
			err_sys("\nread error");
		}
		close(fd[1][0]);				// close read at child 1 from parent 
		for (char *token = strtok(buff1," \n"); token != NULL; token = strtok(NULL, " \n")) {
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
// Parent process that writes buff1 to both of its children 
//=============================================================================================
	// Parent process
	close(fd[0][0]);	// close read at child0 from parent
	close(fd[1][0]);	// close read at child 1 from parent 
	if(write(fd[0][1],buff1,BUFFSIZE)<0){
		err_sys("\nwrite error");
	}
	close(fd[0][1]);	// close write at parent to child0
	if(write(fd[1][1],buff1,BUFFSIZE)<0){
		err_sys("\nwrite error");
	}
	close(fd[1][1]);	// close write at parent to child1
	return 0; 
}

