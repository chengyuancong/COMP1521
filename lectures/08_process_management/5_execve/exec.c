#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
   pid_t pid;
   int stat;

   printf("Parent process starts ...\n");
   if ((pid = fork()) != 0) {
      printf("Parent is waiting ...\n");
      wait(&stat);
      printf("Child status = %08x %d\n", stat,WEXITSTATUS(stat));
   }
   else {
      int   i = 0;
      char  line[50];       // input line buffer
      char *args[100];      // cmd line args
      printf("Give me the argv list (1 per line):\n");
      while (fgets(line, 50, stdin) != NULL) {
         line[strlen(line)-1] = '\0'; // strip '\n'
         //args[i++] = line; //WRONG! why?
         args[i++] = strdup(line); //a malloc then a strcpy
      }
      args[i] = NULL;
      stat = execve(args[0], args, NULL);
      // only reach here if exec fails
      printf("Tried to exec %s\n", args[0]);
      perror("Exec failed");
	  //free the strdup strings.
      return 1;
   }
   return 0;
}