#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
   pid_t pid;
   pid = fork();
   if (pid == 0)
      printf("I am the child.\n");
   else {
      printf("I am the parent.\n");
      kill(pid, SIGKILL);
   }
   return 0;
}