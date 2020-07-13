#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
   pid_t pid;
   int stat;

   printf("Parent process (%d) starts ...\n", getpid());
   if ((pid = fork()) != 0) {
      printf("Parent is waiting ...\n");
      pid = wait(&stat);
      printf("Child (%d) status = %08x\n", pid, stat);
      printf("%d\n",stat);
      if(WIFEXITED(stat)){
          printf("Exit Status was %d\n",WEXITSTATUS(stat));
      }
   }
   else {
      printf("Child process (%d) has parent %d\n",
             getpid(), getppid());

      char *args[2]; 
      args[0] = "./hello";
      args[1] = NULL;
      stat = execve(args[0], args, NULL);
      printf("In child %d\n",stat);
      // only reach here if exec fails
      perror("Exec failed");
      return 1;
   }
   return 0;
}