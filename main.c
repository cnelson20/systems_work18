#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1
#define PARENT_TO_CHILD 0
#define CHILD_TO_PARENT 1

void strtoupper(char *string) {
	while (*string) {
		if (*string >= 'a' && *string <= 'z') {
			*string -= ('a' - 'A');
		}
		string++;
	}
}

int main() {
  int fds[2][2];
  pipe(fds[PARENT_TO_CHILD]);
  pipe(fds[CHILD_TO_PARENT]);	     
  
  char input[4096];
  if (fork()) {
    while (1) {
      printf(": ");
      fgets(input,sizeof(input),stdin);
      unsigned int dtlen = strlen(input)+1;
      write(fds[PARENT_TO_CHILD][WRITE],&dtlen,4);
      write(fds[PARENT_TO_CHILD][WRITE],input,dtlen);
      int childstatus;
      //printf("Waiting for child ... \n");
      //printf("Back!\n");
      read(fds[CHILD_TO_PARENT][READ],input,dtlen);
      printf("Processed text: %s",input);
    }
  } else {
    // child 
    //printf("This is the child!\n");
    unsigned int dtlen;
    while (1) {
      read(fds[PARENT_TO_CHILD][READ],&dtlen,4);
      read(fds[PARENT_TO_CHILD][READ],input,dtlen);
      strtoupper(input);
      write(fds[CHILD_TO_PARENT][WRITE],input,dtlen);
    }
  }
}
