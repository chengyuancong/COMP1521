// Catch a SIGINT signal

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void myHandler(int sigID)
{
   // we don't normally put
   // printf() in a signal handler
   printf("Caught SIGINT\n");
   //sleep(3);
   printf("Finished SIGINT\n");
}
 
int main(int argc, char **argv)
{
  
   if ( signal(SIGINT, myHandler) == SIG_ERR) {
      printf("Can't set signal handler\n");
      exit(1);
   }
   // wait for signal
   pause();
   printf("After the signal was received\n");
   pause();
   //about to execute this next line
   return 0;
}