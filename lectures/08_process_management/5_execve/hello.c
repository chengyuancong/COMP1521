#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/wait.h>
//#include <sys/types.h>

int main(void)
{
   printf("Hello! ");
   printf("Exec'd process (%d) has parent %d\n",
          getpid(), getppid());
   return 0;
}