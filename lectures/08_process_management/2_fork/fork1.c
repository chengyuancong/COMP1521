#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pid_t Fork();

int main(void)
{
   pid_t pid;
   pid = Fork();
   if (pid == 0)
      printf("I am the child.\n");
   else {
      printf("I am the parent.\n");
   }
   return 0;
}

pid_t Fork()
{
   pid_t pid;
   //can fail if system limit is exceeded or user limit is exceeded
   if ((pid = fork()) < 0) {
      perror("fork() failed");
      exit(1);
   }
   return pid;
}