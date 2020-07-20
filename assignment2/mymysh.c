// mysh.c ... a small shell
// Started by John Shepherd, September 2018
// Completed by Yuancong, July 2020

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <glob.h>
#include <assert.h>
#include <fcntl.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(char *);

// Function forward references

void trim(char *);
int strContains(char *, char *);
char **tokenise(char *, char *);
char **fileNameExpand(char **);
void freeTokens(char **);
char *findExecutable(char *, char **);
int isExecutable(char *);
void prompt(void);
void pwd(void);

// Global Constants
#define MAXLINE 200

// Global Data

/* none ... unless you want some */


// Main program
// Set up enviroment and then run main loop
// - read command, execute command, repeat

int main(int argc, char *argv[], char *envp[])
{
   pid_t pid;   // pid of child process
   int stat;    // return status of child
   char **path; // array of directory names
   int cmdNo;   // indicate which command number to get
   int seqNo;   // seqence number of the last entry
   int i;       // generic index

   // set up command PATH from environment variable
   for (i = 0; envp[i] != NULL; i++) {
      if (strncmp(envp[i], "PATH=", 5) == 0) break;
   }
   if (envp[i] == NULL)
      path = tokenise("/bin:/usr/bin",":");
   else
      // &envp[i][5] skips over "PATH=" prefix
      path = tokenise(&envp[i][5],":");
      
#ifdef DBUG
   for (i = 0; path[i] != NULL;i++)
      printf("path[%d] = %s\n",i,path[i]);
#endif

   // initialise command history
   // - use content of ~/.mymysh_history file if it exists
   seqNo = initCommandHistory();

   // main loop: print prompt, read line, execute command

   char line[MAXLINE];
   char **args;
   char *fullpath;
   int success = 0;
   prompt();
   while (fgets(line, MAXLINE, stdin) != NULL) {
      printf("%s", line);

      // remove leading/trailing space
      trim(line);

      // if command is "exit", exit
      if (!strcmp(line,"exit")) break;

      // if empty command, ignore
      if (!strcmp(line, "")) {
         prompt();
         continue;
      }

      // handle ! history substitution
      if (line[0] == '!') {
         if (line[1] == '!') {
            if (seqNo != 0) {
               strcpy(line, getCommandFromHistory(seqNo));
            } else {
               printf("No command #0\n");
               prompt();
               continue;
            }           
         } else {
            if (sscanf(line, "!%d", &cmdNo) == 1) {
               if (getCommandFromHistory(cmdNo) != NULL) {
                  strcpy(line, getCommandFromHistory(cmdNo));
               } else {
                  printf("No command #%d\n", cmdNo);
                  prompt();
                  continue;
               }
            } else {
               printf("Invalid history substitution\n");
               prompt();
               continue;
            }
         }
      }
      
      // tokenise
      args = tokenise(line, " ");

      // handle *?[~ filename expansion
      args = fileNameExpand(args);

      // handle shell built-ins
      if (!strcmp(args[0], "h") || !strcmp(args[0], "history")) {
         showCommandHistory(stdout);
         seqNo++;
         addToCommandHistory(line, seqNo);
         prompt();
         continue;
      }

      if (!strcmp(args[0], "pwd")) {
         pwd();
         seqNo++;
         addToCommandHistory(line, seqNo);
         prompt();
         continue;
      }

      if (!strcmp(args[0], "cd")) {
         if (!chdir(args[1])) {
            pwd();
            seqNo++;
            addToCommandHistory(line, seqNo);
         } else {
            printf("%s: No such file or directory\n", args[1]);
         }
         prompt();
         continue;  
      }

      // check for input/output redirections
      
      // find executable using first token
      fullpath = findExecutable(args[0], path);

      // if none, then Command not found
      if (fullpath == NULL) {
         printf("%s: Command not found\n", args[0]);
         prompt();
         continue;
      }

      // sort out any redirections

      // run the command
      printf("Running %s ...\n", fullpath);
      printf("--------------------\n");

      pid = fork();
      if (pid > 0) {
         wait(&stat);
      } else if (pid == 0) {
         success = execve(fullpath, args, envp);
      } else {
         perror("Fork error: ");
      }

      printf("--------------------\n");
      if (success == 0) {
         printf("Returns 0\n");
         seqNo++;
         addToCommandHistory(line, seqNo);
      } else {
         printf("Returns 1\n");
      }
      
      // print prompt
      prompt();
   }
   saveCommandHistory();
   cleanCommandHistory();
   freeTokens(path);
   freeTokens(args);
   free(fullpath);
   printf("\n");
   return(EXIT_SUCCESS);
}

// fileNameExpand: expand any wildcards in command-line args
// - returns a possibly larger set of tokens
char **fileNameExpand(char **tokens)
{  
   glob_t buffer;
   int i = 0;
   if (tokens[i] != NULL) {
      glob(tokens[i], GLOB_NOCHECK|GLOB_TILDE, NULL, &buffer);
      i++;
   }
   for ( ; tokens[i] != NULL; i++) {
      glob(tokens[i], GLOB_NOCHECK|GLOB_TILDE|GLOB_APPEND, NULL, &buffer);
   }
   tokens = realloc(tokens, (buffer.gl_pathc + 1) * sizeof(char *));
   assert(tokens != NULL);
   for (i = 0; i < buffer.gl_pathc; i++) {
      tokens[i] = strdup(buffer.gl_pathv[i]);
   }
   tokens[i] = NULL;
   globfree(&buffer);
   return tokens;
}


// findExecutable: look for executable in PATH
char *findExecutable(char *cmd, char **path)
{
      char executable[MAXLINE];
      executable[0] = '\0';
      if (cmd[0] == '/' || cmd[0] == '.') {
         strcpy(executable, cmd);
         if (!isExecutable(executable))
            executable[0] = '\0';
      } else {
         int i;
         for (i = 0; path[i] != NULL; i++) {
            sprintf(executable, "%s/%s", path[i], cmd);
            if (isExecutable(executable)) break;
         }
         if (path[i] == NULL) executable[0] = '\0';
      }
      if (executable[0] == '\0')
         return NULL;
      else
         return strdup(executable);
}

// isExecutable: check whether this process can execute a file
int isExecutable(char *cmd)
{
   struct stat s;
   // must be accessible
   if (stat(cmd, &s) < 0)
      return 0;
   // must be a regular file
   //if (!(s.st_mode & S_IFREG))
   if (!S_ISREG(s.st_mode))
      return 0;
   // if it's owner executable by us, ok
   if (s.st_uid == getuid() && s.st_mode & S_IXUSR)
      return 1;
   // if it's group executable by us, ok
   if (s.st_gid == getgid() && s.st_mode & S_IXGRP)
      return 1;
   // if it's other executable by us, ok
   if (s.st_mode & S_IXOTH)
      return 1;
   return 0;
}

// tokenise: split a string around a set of separators
// create an array of separate strings
// final array element contains NULL
char **tokenise(char *str, char *sep)
{
   // temp copy of string, because strtok() mangles it
   char *tmp;
   // count tokens
   tmp = strdup(str);
   int n = 0;
   strtok(tmp, sep); n++;
   while (strtok(NULL, sep) != NULL) n++;
   free(tmp);
   // allocate array for argv strings
   char **strings = malloc((n+1)*sizeof(char *));
   assert(strings != NULL);
   // now tokenise and fill array
   tmp = strdup(str);
   char *next; int i = 0;
   next = strtok(tmp, sep);
   strings[i++] = strdup(next);
   while ((next = strtok(NULL,sep)) != NULL)
      strings[i++] = strdup(next);
   strings[i] = NULL;
   free(tmp);
   return strings;
}

// freeTokens: free memory associated with array of tokens
void freeTokens(char **toks)
{
   for (int i = 0; toks[i] != NULL; i++)
      free(toks[i]);
   free(toks);
}

// trim: remove leading/trailing spaces from a string
void trim(char *str)
{
   int first, last;
   first = 0;
   while (isspace(str[first])) first++;
   last  = strlen(str)-1;
   while (isspace(str[last])) last--;
   int i, j = 0;
   for (i = first; i <= last; i++) str[j++] = str[i];
   str[j] = '\0';
}

// strContains: does the first string contain any char from 2nd string?
int strContains(char *str, char *chars)
{
   for (char *s = str; *s != '\0'; s++) {
      for (char *c = chars; *c != '\0'; c++) {
         if (*s == *c) return 1;
      }
   }
   return 0;
}

// prompt: print a shell prompt
// done as a function to allow switching to $PS1
void prompt(void)
{
   printf("mymysh$ ");
}

// pwd: print current working directory
void pwd(void) {
   char buffer[MAXLINE];
   getcwd(buffer, sizeof(buffer));
   printf("%s\n", buffer);
}