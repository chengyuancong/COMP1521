#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//ps x -o user,pid,pgid,args
//ps -u dp1092 -o pid,pgid,args

//ps -o pid,euid,ruid,suid,egid,rgid,sgid,cmd



int main(void)
{
   printf("Starting process...\n");
   printf("%u\n", getuid());
   sleep(30);
   printf("P: my pid is %d.\n", getpid());
   printf("P: my parent is %d.\n", getppid());
   return 0;
}