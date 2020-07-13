#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
   int status;
   pid_t pid;
   pid = fork();
   if (pid == 0) {
      sleep(20);
      printf("I am the child.\n");
      return 2;
   }
   else {
      wait(&status);
      if(WIFEXITED(status)){
         printf("Status was %d %d %08x\n",WEXITSTATUS(status), status,status);
      } else {
	     printf("Did not exit %d %08x\n",status,status);
      }
      printf("I am the parent.\n");
   }
   return 0;
}