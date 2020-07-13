#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(void)
{
   pid_t pid = fork();

   if (pid != 0) {
      sleep(2);
	  
      printf("I am the parent.\n");
      printf("P: my pid is %d.\n", getpid());
      printf("P: my child is %d.\n", pid);
      printf("P: my parent is %d.\n", getppid());
      printf("P: my group is %d.\n", getpgid(0));
   }
   else {
      // for(int i = 0; i < 100000; i++)
      //for(int j = 0; j < 100000; j++)
			;
      printf("I am the child.\n");
      printf("C: my pid is %d.\n", getpid());
      printf("C: my parent is %d.\n", getppid());
      printf("C: my group is %d.\n", getpgid(0));
   }
   return 0;
}