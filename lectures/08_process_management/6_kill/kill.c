#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
   pid_t pid;
   pid = fork();
   if (pid == 0) {
      printf("I am the child.\n");
      sleep(1);
      printf("Child wakes up.\n");
   }
   else {
      printf("I am the parent.\n");
      sleep(100);
      if( kill(pid, SIGKILL) < 0 ) fprintf(stderr,"Kill failed.\n");
	  printf("Tried to kill zombie\n");
      sleep(20);
      printf("Parent finishes.\n");
   }
   return 0;
}