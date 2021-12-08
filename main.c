#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1
#define PARENT_TO_CHILD 0
#define CHILD_TO_PARENT 1

// Functions
void strtolower(char *string);
void leetify(char *string);
int main();

// Globals
int fds[2][2];

int main() {
  pipe(fds[PARENT_TO_CHILD]);
  pipe(fds[CHILD_TO_PARENT]);	     
  
  char input[4096];
  if (fork()) {
    while (1) {
      printf(": ");
      fgets(input,sizeof(input),stdin);
      *strrchr(input,'\n') = '\0';
      unsigned int dtlen = strlen(input)+1;
      write(fds[PARENT_TO_CHILD][WRITE],&dtlen,4);
      write(fds[PARENT_TO_CHILD][WRITE],input,dtlen);
      int childstatus;
      //printf("Waiting for child ... \n");
      //printf("Back!\n");
      read(fds[CHILD_TO_PARENT][READ],input,dtlen);
      printf("Processed text: %s\n",input);
    }
  } else {
    // child 
    //printf("This is the child!\n");
    unsigned int dtlen;
    while (1) {
      read(fds[PARENT_TO_CHILD][READ],&dtlen,4);
      read(fds[PARENT_TO_CHILD][READ],input,dtlen);

      leetify(input);

      write(fds[CHILD_TO_PARENT][WRITE],input,dtlen);
    }
  }

  return 0;
}


// Make string lowercase
void strtolower(char *string) {
  while (*string) {
    if (*string >= 'A' && *string <= 'Z') {
      *string += *string + ('a' - 'A');
    }
    string++;
  }
}

// Leetify String;
void leetify(char *string) {
  strtolower(string);
  
  int i;
  int stlen = strlen(string) - 1;
  for (i = 0; i <= stlen / 2; i++) {
    int tmp = string[i];
    string[i] = string[stlen-i];
    string[stlen-i] = tmp;
  }
  zs = *string;
  while (*string) {
    switch (*string) {
    case 'a':
      replace_string_inplace(zs,"a","4");
      break;
    case 'b':
      replace_string_inplace(zs,"b","6");
      break;
    case 'l':
      replace_string_inplace(zs,"l","7");
      break;
    }
    string++;
  }
}

char replace_string_inplace(char *haystack,char *needle, char *toreplace) {
  char *temp = replace_string( haystack,needle,toreplace);
  if (temp != NULL) {
    realloc(haystack,strlen(temp)+1);
    strcpy(haystack,temp);
  }
}
  
char *replace_string(char *haystack, char *needle, char *toreplace) {
  //printf("'%s' '%s' '%s'\n",haystack,needle,toreplace);
  char *tmpchr, *tmpndl, *lpchr;
  tmpchr = haystack;
  while (*tmpchr) {
    lpchr = tmpchr;
    tmpndl = needle;
	
	// while letters of haystack and needle match
    while (*lpchr == *tmpndl) {
      lpchr++;
      tmpndl++;
	  // if end of needle (success), break
      if (!(*tmpndl)) {
	    break;
      }
	  // if end of string reached, return null
      if (!(*lpchr)) {
	    return NULL;
      }
    }
	// if end of while loop was because end of needle, break
    if (!(*tmpndl)) {
      break;
    }
    tmpchr++;
  }
  // if end of string was reached return NULL
  if (tmpchr - haystack >= strlen(haystack)) {
    return NULL;
  }
  char *half2, *new;
  int sizehalf1, sizehalf2, sizenew;
  // Calculate sizes of string before and after matching part
  sizehalf1 = tmpchr - haystack;
  sizehalf2 = strlen(haystack) - (tmpchr - haystack) - strlen(needle);
  // calc size of new string 
  sizenew = sizehalf1 + sizehalf2 + strlen(toreplace);
  //printf("sizehalf1: %d sizehalf2: %d sizenew: %d\n",sizehalf1,sizehalf2,sizenew);
  new = malloc(sizenew+1);
  // Copy half before, what to replace, and half after 
  memcpy(new,haystack,sizehalf1);
  memcpy(new+sizehalf1,toreplace,strlen(toreplace));
  memcpy(new+sizehalf1+strlen(toreplace),tmpchr+strlen(needle),sizehalf2);
  // Null-terminate string 
  //printf("sizehalf1: %d strlen(toreplace): %d sizehalf2: %d\n",sizehalf1,strlen(toreplace),sizehalf2);
  *(new+sizehalf1+strlen(toreplace)+sizehalf2) = '\0';
  //printf("Haystack: '%s'\nNew: '%s'\n",haystack,new);
  return new;
}
