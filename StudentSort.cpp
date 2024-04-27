#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

int main(int argc, char* args[]){
	int stdfd;
	int inputFile[argc-2];
	int k=0;
	stdfd = open(args[1], O_RDONLY);
    	if (stdfd == -1) {
        	perror("Error opening input file");
       		return 0;
	}
	for(int i=2; i < argc;i++){
		inputFile[k] = open(args[i], O_WRONLY);
    		if (inputFile[k] == -1) {
        		perror("Error opening input file");
       			return 0;
		}
		k++;
	}
	char num;
	int j = 0;
	
	ssize_t bytes_read;
	bytes_read = read(stdfd, &num, 1);
	if (bytes_read == -1) {
        	perror("read");
        	close(stdfd);
        	return 1;
    	} 
    	
    	while(j < atoi(&num)){
    		char input[19];
    		int fd[2];
    		pipe(fd);
    		pid_t pid = fork();
    		if(pid == 0){
    			char inputc[19];
    			char temp[14];
    			read(fd[0], &inputc, 19);
    			//cout << inputc << endl;
    			char section = inputc[18];
    			int sec = atoi(&section);
    			//cout << sec << endl;
    			for(int k=0; k < 14;k++){
    				temp[k]= inputc[k+1];
    			}
    			temp[13] = '\n';
    			
    			ssize_t bytesWritten;
    			bytesWritten = write(inputFile[sec - 1], &temp, 14);
    			//cout << "Parent ID: " << getppid() << endl;
    			//cout << "Child ID: " << getpid() << endl;
    			exit(0);
    		}
    		else{
    			bytes_read = read(stdfd, &input, 19);
    			if (bytes_read == -1) {
       				perror("read");
       				close(stdfd);
       				return 1;
  			} 
  			write(fd[1], &input, 19);
  		
    			wait(NULL);
    			//j++;
    		}
    		j++;
    	}
	
	close(stdfd);
	for(int i=0; i<argc-2;i++){
		close(inputFile[i]);
	}
	return 0;
}
