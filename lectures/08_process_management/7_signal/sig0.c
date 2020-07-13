// Catch a SIGINT signal
// Type a Ctrl-C at runtime to send the SIGINT signal
// Or run as ./sig0 & and use
// kill -INT pid
// Or kill -INT %jobid
//Use ps to see processids and jobs to see jobids

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void myHandler(int sigID)
{
   // we don't normally put
   // printf() in a signal handler
   printf("Caught SIGINT\n");
   exit(0);
}

int main(int argc, char **argv)
{
   if (signal(SIGINT, myHandler) == SIG_ERR) {
      printf("Can't set signal handler\n");
      exit(1);
   }
   sleep(20); // wait for signal
   printf("No signal received\n");
   return 0;
}