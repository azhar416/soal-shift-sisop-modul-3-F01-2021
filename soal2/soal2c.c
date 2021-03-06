#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int pid;
int pipe1[2];
int pipe2[2];
void exec1();
void exec2();
void exec3();

int main() {

  // create pipe1
  if (pipe(pipe1) == -1) {
    perror("bad pipe1");
    exit(1);
  }
    
  // fork (ps aux)
  if ((pid = fork()) == -1) {
    perror("bad fork1");
    exit(1);
  } 
  
  else if (pid == 0) 
  {
    // stdin --> ps --> pipe1
    exec1();
  }
  // parent

  // create pipe2
  if (pipe(pipe2) == -1) {
    perror("bad pipe2");
    exit(1);
  }

  // fork (grep root)
  if ((pid = fork()) == -1) {
    perror("bad fork2");
    exit(1);
  } else if (pid == 0) {
    // pipe1 --> grep --> pipe2
    exec2();
  }
  // parent

  // close unused fds
  close(pipe1[0]);
  close(pipe1[1]);

  // fork (grep sbin)
  if ((pid = fork()) == -1) {
    perror("bad fork3");
    exit(1);
  } else if (pid == 0) {
    // pipe2 --> grep --> stdout
    exec3();
  }
  // parent


}


void exec1() {
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  // exec
  execlp("ps", "ps", "aux", NULL);
  //char *argv[]= {"ps","aux", NULL};
  //execv("bin/ps", argv);
  // exec didn't work, exit
  perror("bad exec ps");
  _exit(1);
}

void exec2() {
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);
  // close fds
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("sort", "sort", "-nrk", "3.3", NULL);
  //char *argv[]= {"sort","-nrk","3.3", NULL};
  //execv("/bin/sort",argv);
  // exec didn't work, exit
  perror("bad exec grep root");
  _exit(1);
}

void exec3() {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)
  // close fds
  close(pipe2[0]);
  close(pipe2[1]);
  // exec
  execlp("head", "head", "-5", NULL);
  //char *argv[]= {"head","-5", NULL};
  //execv("/bin/head",argv);
  // exec didn't work, exit
  perror("bad exec grep sbin");
  _exit(1);
}
