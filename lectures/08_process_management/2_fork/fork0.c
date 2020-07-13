#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
   pid_t pid;
   pid = fork();
   if (pid < 0)
      perror("I am the parent and fork() failed");
   else if (pid == 0){
      sleep(100);
      printf("I am the child.\n");
   }else {
      sleep(50); //sleep for 1 second
      printf("I am the parent.\n");
     
   }
   return 0;
}