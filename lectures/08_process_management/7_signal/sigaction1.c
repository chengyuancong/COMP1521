// Example of using sigaction() to set up
// a signal handler
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


void handler(int sig)
{
   printf ("Got signal %d\n", sig);
}
 
int main(int argc, char *argv[])
{
   
   struct sigaction act; 
 
   //To overwrite all the garbage
   //This is important since we won't be setting
   //all of the fields.
   memset (&act, 0, sizeof(act));
 
   // Use the sa_sigaction field because
   // the handler has two additional parameters
   act.sa_handler = handler;

   printf("%d %d %d\n",SIGHUP, SIGTERM, SIGINT);
   if (sigaction(SIGHUP, &act, NULL) < 0) {
      perror ("sigaction");
      return EXIT_FAILURE;
   }
 
   if (sigaction(SIGTERM, &act, NULL) < 0) {
      perror ("sigaction");
      return EXIT_FAILURE;
   }
   
   if (sigaction(SIGINT, &act, NULL) < 0) {
      perror ("sigaction");
      return EXIT_FAILURE;
   }
 
   pause();
 
   return EXIT_SUCCESS;
}  