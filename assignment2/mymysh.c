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
#include <errno.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(char *);

// Function forward references

void trim(char *);
char **tokenise(char *, char *);
char **fileNameExpand(char **);
void freeTokens(char **);
char *findExecutable(char *, char **);
int isExecutable(char *);
void prompt(void);
void pwd(void);
int checkRedirect(char **, int);
int howManyTokens(char **);
int checkInput(char *);
int checkOutput(char *);
void redirect(char **, int);

// Global Constants
#define MAXLINE 200

#define VALID   0
#define INVALID 1
#define NONE    2


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

   char line[MAXLINE];     // input line buffer
   char **args;            // arguements holding tokenised lines
   char *fullpath;         // fullpath of executable
   int length = 0;         // numbers of tokens
   int redirctStatus = 0;  // hold a value which determine whther to redirect
   
   prompt();
   while (fgets(line, MAXLINE, stdin) != NULL) {

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
         // !!
         if (line[1] == '!') {
            if (seqNo != 0) {
               strcpy(line, getCommandFromHistory(seqNo));
               printf("%s\n", line);
            } else {
               printf("No command #0\n");
               prompt();
               continue;
            }           
         } else {
            // !#
            if (sscanf(line, "!%d", &cmdNo) == 1) {
               if (getCommandFromHistory(cmdNo) != NULL) {
                  strcpy(line, getCommandFromHistory(cmdNo));
                  printf("%s\n",line);
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
         freeTokens(args);
         continue;
      }

      if (!strcmp(args[0], "pwd")) {
         pwd();
         seqNo++;
         addToCommandHistory(line, seqNo);
         prompt();
         freeTokens(args);
         continue;
      }

      if (!strcmp(args[0], "cd")) {
         // cd -> "HOME"
         if (args[1] == NULL) {
            chdir(getenv("HOME"));
            pwd();
            seqNo++;
            addToCommandHistory(line, seqNo);
         // cd path
         } else {
            if (chdir(args[1]) == 0) {
               pwd();
               seqNo++;
               addToCommandHistory(line, seqNo);
            } else {
               printf("%s: %s\n", args[1],strerror(errno));   
            }
         }
         prompt();
         freeTokens(args);
         continue;  
      }

      // check for input/output redirections
      length = howManyTokens(args);
      
      redirctStatus = checkRedirect(args, length);
      if (redirctStatus == INVALID) {
         prompt();
         freeTokens(args);
         continue;
      }
      
      // find executable using first token
      fullpath = findExecutable(args[0], path);

      // if none, then Command not found
      if (fullpath == NULL) {
         printf("%s: Command not found\n", args[0]);
         freeTokens(args);
         free(fullpath);
         prompt();
         continue;
      }

      // run the command
      printf("Running %s ...\n", fullpath);
      printf("--------------------\n");

      pid = fork();
      if (pid > 0) {
         wait(&stat);
      } else if (pid == 0) {
         
         // sort out redirections
         if (redirctStatus == VALID) {
            redirect(args, length);
         }
         
         execve(fullpath, args, envp);
         printf("%s: unknown type of executable\n", args[0]);
         exit(-1);

      } else {
         perror("Fork error");
      }

      printf("--------------------\n");
      printf("Returns %d\n", WEXITSTATUS(stat));
      seqNo++;
      addToCommandHistory(line, seqNo);
      
      // print prompt
      prompt();
      freeTokens(args);
      free(fullpath);
   }
   saveCommandHistory();
   cleanCommandHistory();
   freeTokens(path); 
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
   freeTokens(tokens);
   tokens = malloc((buffer.gl_pathc + 1) * sizeof(char *));
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
void freeTokens(char **tokens)
{
   for (int i = 0; tokens[i] != NULL; i++)
      free(tokens[i]);
   free(tokens);
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


// howManyTokens: count how many tokens are in char **tokens
int howManyTokens(char **tokens)
{
   int length = 0;
   while (tokens[length] != NULL) {
      length++;
   }
   return length;
}


// checkRedirect: check if the redirection can be executed
// return VALID if redirection can be done
// return INVALD if redirction cannot be done
// return NONE if not redirction is detected
int checkRedirect(char **tokens, int length)
{  
   // if number of tokens is <= 1 and ">" or "<" appears, definitly invalid
   if (length < 2) {
      if (!strcmp(tokens[0], ">") || !strcmp(tokens[0], "<")) {
         printf("Invalid i/o redirection\n");
         return INVALID;
      } else {
         return NONE;
      }
   }
   // if >= 2, check if it is in right place
   for (int i = 0; tokens[i] != NULL; i++) {
      if ((!strcmp(tokens[i], ">") || !strcmp(tokens[i], "<")) 
           && (i != length - 2 || i == 0)) {
         printf("Invalid i/o redirection\n");
         return INVALID;
      }
   }
   // if right, check if the objected file is accessable
   if (!strcmp(tokens[length-2], "<")) {
      if (!checkInput(tokens[length-1])) {
         return VALID;
      } else {
         return INVALID;
      }
   } else if (!strcmp(tokens[length-2], ">")) {
      if (!checkOutput(tokens[length-1])) {
         return VALID;
      } else {
         return INVALID;
      }
   // if no ">" or "<", return NONE
   } else {
      return NONE;
   }
}  


// checkInput: check if the input document exist or can be access
int checkInput(char *arg)
{
   FILE *input = fopen(arg, "r");
   if (input != NULL) {
      fclose(input);
      return 0;
   } else {
      perror("Input redirection");
      return 1;
   }
}


// checkOutput: check if the output ducument can be access
int checkOutput(char *arg)
{
   FILE *output = fopen(arg, "w");
   if (output != NULL) {
      fclose(output);
      return 0;
   } else {
      perror("Output redirection");
      return 1;
   }
}


// redirct: redirect stdin/stdout to objected ducuments
void redirect(char **tokens, int length)
{
   if (!strcmp(tokens[length-2], "<")) {
      int inputFd = open(tokens[length-1], O_RDONLY);
      dup2(inputFd, STDIN_FILENO);
      free(tokens[length-1]);
      free(tokens[length-2]);
      tokens[length-2] = NULL;
   } else if (!strcmp(tokens[length-2], ">")) {
      int outputFd = open(tokens[length-1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
      dup2(outputFd, STDOUT_FILENO);
      free(tokens[length-1]);
      free(tokens[length-2]);
      tokens[length-2] = NULL;
   }
}
