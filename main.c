#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

void strtoupper(char *string) {
	while (*string) {
		if (*string >= 'a' && *string <= 'z') {
			*string -= ('a' - 'A');
		}
		string++;
	}
}

int main() {
	int fds[2];
	pipe(fds);
	
	char input[4096];
	while (1) {
		if (fork()) {
			printf(": ");
			fgets(input,sizeof(input),stdin);
			unsigned int dtlen = strlen(input)+1;
			write(fds[WRITE],&dtlen,4);
			write(fds[WRITE],input,dtlen);
			int childstatus;
			//printf("Waiting for child ... \n");
			wait(&childstatus);
			//printf("Back!\n");
			read(fds[READ],input,dtlen);
			printf("Processed text: %s",input);
		} else {
			// child 
			//printf("This is the child!\n");
			unsigned int dtlen;
			read(fds[READ],&dtlen,4);
			read(fds[READ],input,dtlen);
			strtoupper(input);
			write(fds[WRITE],input,dtlen);
			exit(0);
		}
	}
}