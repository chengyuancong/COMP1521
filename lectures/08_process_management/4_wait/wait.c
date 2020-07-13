#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
   pid_t pid;
   pid = fork();
   if (pid == 0) {
      sleep(10);
      printf("I am the child.\n");
   }
   else {
      wait(NULL);
      printf("I am the parent.\n");
   }
   return 0;
}