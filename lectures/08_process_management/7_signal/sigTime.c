// Signal a sleeping process

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef unsigned int uint;

void wakeUp(int sigID)
{
   printf("Awake!\n");
   // signal handler actually does nothing
   return;
}

uint snooze(uint secs)
{
   printf("Starting my snooze...\n");
   uint remaining = sleep(secs);
   printf("Slept for %d of %d secs\n",
                 secs-remaining, secs);
   return remaining;
}

int main(int argc, char **argv)
{
   if (argc < 2) {
      printf("Usage: %s seconds\n", argv[0]);
      exit(0);
   }
   if (signal(SIGINT, wakeUp) == SIG_ERR) {
      printf("Can't set signal handler\n");
      exit(1);
   }
   snooze(atoi(argv[1]));
   return 0;
}