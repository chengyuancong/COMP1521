// Catch a SIGINT signal
// Type a Ctrl-C at runtime to send the SIGINT signal

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
   if (signal(SIGINT, SIG_IGN) == SIG_ERR) {
      printf("Can't set signal handler\n");
      exit(1);
   }
   sleep(3); // wait for signal
   printf("No signal received\n");
   return 0;
}