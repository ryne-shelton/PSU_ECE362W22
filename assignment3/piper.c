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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFSIZE 	4096

void err_sys(char *msg)
{
	fprintf(stderr,"%s (%s)\n",msg,strerror(errno));
	exit(-1);
}

int main()
{
	// variable declarations
	char	buff0[BUFFSIZE], buff1[2*BUFFSIZE];  // two data buffers for passing things
	// fd[0] - parent/child0 pipe, fd[1] - parent/child1 pipe
	int 	fd[2][2], nread; // fd[][0] - read , fd[][1] - write
	
	// read input into buffer until ctrl-d ** needs work 
	while((nread=read(STDIN_FILENO, buff0, BUFFSIZE)) > 0){
		strcat(buff1,buff0);					
		printf("\nbuff1: %s\n",buff1);
		printf("\nbuff0: %s\n",buff0);
		continue;
	}
	if(nread<0) err_sys("read error"); 
	
	for(int i=0;i<2;i++){
		if(pipe(fd[i])<0) err_sys("\npipe error");	// open pipes
	}
	int child0 = fork();	// Creates child0
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
 		printf("\nBuffer read in from Parent in Child0 process: %s\n",buff1);	// print was was read from parent 
		//add code here to perform addition on buffer
		return 0;
	}	
	int child1 = fork();	// Creates child1
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
		printf("\nBuffer read in from Parent in Child1 process: %s\n",buff1);	// print was was read from parent
		// add code here to perfrom multiplication 
		return 0;
	}
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

/* 
int main(int argc, char *argv[]){
    int n, fd[2], line1[MAXLINE];
    pid_t Child1, Child2;
    char *line[MAXLINE]; 
    
    if (pipe(fd) < 0){ // make pipe (parent) 
		err_sys("\npipe error");
	}      
    if ((Child1 = fork()) < 0){    // fork it
        err_sys("\nfork error");
    } else if (Child1 > 0){ 
        //int num = read(fd[0], line , MAXLINE); // parent reading from pipe 
        close(fd[0]); // parent closes reading end
        for (int i=0; i<argc-1; i++){
		    strcat(line,*++argv); // adds arguements to string
        }
        write(fd[1], line , sizeof(line)); // parent writes to pipe
    } else{
        close(fd[1]); // child1 closes writing end
        n = read(fd[0], line, MAXLINE); // child1 reads from the pipe
        char* token = strtok(line," \n");
        while ((token != NULL)){
                int number = atoi(token); 
                printf("%d\n", number);
                token = strtok(NULL," \n");
        }
        //for (int i = 0; i <= strlen(*line); i++){
        //    if ((strcmpline[i] =="\n") || (line[i] == " ")){
        //        line[i++];
        //        line1[i] =+ atoi(line[i]);
        //    }
        //    line1[i] =+ atoi(line[i]);
        //} 
        write(fd[1], line1, n); //child1 writes to pipe
    }
    exit(0);
    //printf("hello world (pid:%d)\n", (int) getpid());
    //int rc = fork();
    //if (rc < 0){    // fork failed, then exit
    //    fprintf(stderr, "fork failed\n");
    //    exit(1);
    //} else if (rc == 0){     // child (new process)
    //    printf("hello, I am child (pid:%d)\n", (int) getpid());
    //} else {                // parent goes down this path (main)
    //    printf("hello, I am parent of %d (pid:%d)\n",
    //            rc, (int) getpid());
}
 

  */