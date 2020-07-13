#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
   //Child process will get a copy of all the variables that exist
   //in the parent when fork is called.
   int x = 10;
   int *a = malloc(100);
   a[0] = 100;
   pid_t pid;
   pid = fork();
   if (pid < 0)
      perror("fork() failed");
   else if (pid == 0){
      x++;
      a[0]++;
      printf("I am the child. %d %d\n",x,a[0]);
      free(a);
   }else {
      x++;
      a[0]++;
      //sleep(1); sleep for 1 second
      printf("I am the parent. %d %d\n",x,a[0]);
      free(a);
   }
   return 0;
}