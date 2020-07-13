// Example of using sigaction() to set up
// a signal handler
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
 
void handler(int sig)
{
   printf ("Invalid memory reference!!\n");
   exit(1);
}
 
int main(int argc, char *argv[])
{
   struct sigaction act;
 
   memset (&act, 0, sizeof(act));
 
   // Use the sa_sigaction field because
   // the handler has two additional parameters
   act.sa_handler = &handler;

   //if (sigaction(SIGSEGV, &act, NULL) < 0) {
   //   perror ("sigaction");
   //   return EXIT_FAILURE;
   //}
 
   int *p = NULL;
   *p = 3;
   printf("Oh well...\n");
   return EXIT_SUCCESS;
} 