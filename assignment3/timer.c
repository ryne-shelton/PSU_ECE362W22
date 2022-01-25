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
#include <string.h>

int main(int argc, char *argv[]){
	char user_input[50];
	for (int i=0; i<argc-1; i++){
		strcat(user_input," ");
		strcat(user_input,*++argv);
	}
	printf("input: %s\n", user_input);
}