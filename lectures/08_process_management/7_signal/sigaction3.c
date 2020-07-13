// Example of using sigaction() to set up
// a signal handler with 3 arguments including siginfo_t
 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
 
void handler (int sig, siginfo_t *siginfo, void *context)
{
   printf("Signal %d\n",sig);
   printf ("Signal from process %ld owned by user %ld\n",
           (long)siginfo->si_pid, (long)siginfo->si_uid);
}
 
int main (int argc, char *argv[])
{
   struct sigaction act;
 
   memset (&act, '\0', sizeof(act));
 
   // Use the sa_sigaction field because
   // the handler has two additional parameters
   act.sa_sigaction = &handler;
 
   // The SA_SIGINFO flag tells sigaction() to use
   // the sa_sigaction field, not sa_handler
   act.sa_flags = SA_SIGINFO;
 
   
   //Try with Ctrl-C vs kill -SIGINT %?
   if (sigaction(SIGINT, &act, NULL) < 0) {
      perror ("sigaction");
      return EXIT_FAILURE;
   }
 
   pause();
 
   return EXIT_SUCCESS;
}