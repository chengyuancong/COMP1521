// CP1521 mysh ... command history
// Implements an abstract data object

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

// This is defined in string.h
// BUT ONLY if you use -std=gnu99
//extern char *strdup(const char *s);

// Command History
// array of command lines
// each is associated with a sequence number

#define MAXHIST 20
#define MAXSTR  200
#define MAXLINE (MAXSTR+6)

#define HISTFILE "/.mymysh_history"

typedef struct _history_entry {
   int seqNumber;
   char commandLine[MAXSTR];
} HistoryEntry;

typedef struct _history_list {
   int nEntries;
   HistoryEntry commands[MAXHIST];
} HistoryList;

HistoryList CommandHistory;

char histPath[MAXSTR];

// initCommandHistory()
// - initialise the data structure
// - read from .mymysh_history if it exists

int initCommandHistory()
{  
   initHistoryPath();
   FILE *histf = fopen(histPath, "r");
   CommandHistory.nEntries = 0;
   if (histf != NULL) {
      char buffer[MAXLINE];
      while (fgets(buffer, MAXLINE, histf) != NULL) {
         sscanf(buffer, " %d  %[^\n]", 
               &(CommandHistory.commands[CommandHistory.nEntries].seqNumber),
               CommandHistory.commands[CommandHistory.nEntries].commandLine);
         CommandHistory.nEntries++;
      }
      fclose(histf);
   }
   return CommandHistory.nEntries == 0? 
          0:CommandHistory.commands[CommandHistory.nEntries-1].seqNumber;
}

// addToCommandHistory()
// - add a command line to the history list
// - overwrite oldest entry if buffer is full

void addToCommandHistory(char *cmdLine, int seqNo)
{
   if (CommandHistory.nEntries < 20) {
      CommandHistory.commands[CommandHistory.nEntries].seqNumber = seqNo;
      strcpy(CommandHistory.commands[CommandHistory.nEntries].commandLine, cmdLine);
      CommandHistory.nEntries++;
   } else if (CommandHistory.nEntries == 20) {
      int i;
      for (i = 0; i < (CommandHistory.nEntries - 1); i++) {
         CommandHistory.commands[i] = CommandHistory.commands[i+1];
      }
      CommandHistory.commands[i].seqNumber = seqNo;
      strcpy(CommandHistory.commands[i].commandLine, cmdLine);
   }
}

// showCommandHistory()
// - display the list of 

void showCommandHistory(FILE *outf)
{
   for (int i = 0; i < CommandHistory.nEntries; i++) {
      fprintf(outf, " %3d  %s\n", 
             CommandHistory.commands[i].seqNumber, 
             CommandHistory.commands[i].commandLine);
   }
}

// getCommandFromHistory()
// - get the command line for specified command
// - returns NULL if no command with this number

char *getCommandFromHistory(int cmdNo)
{
   if (CommandHistory.nEntries > 0 
      && (cmdNo - CommandHistory.commands[0].seqNumber) >= 0 
      && (cmdNo - CommandHistory.commands[0].seqNumber) < CommandHistory.nEntries) {
      return CommandHistory.commands[cmdNo - CommandHistory.commands[0].seqNumber].commandLine;
   } else {
      return NULL;
   }
}

// saveCommandHistory()
// - write history to $HOME/.mymysh_history

void saveCommandHistory()
{  
   FILE *histf = fopen(histPath, "w");
   showCommandHistory(histf);
   fclose(histf);
}

// cleanCommandHistory
// - release all data allocated to command history

void cleanCommandHistory()
{
   for (int i = 0; i < CommandHistory.nEntries; i++) {
      CommandHistory.commands[i].seqNumber = 0;
      strcpy(CommandHistory.commands[i].commandLine, "");
   }
   CommandHistory.nEntries = 0;
}

// expand the path of /.mymysh_history

void initHistoryPath() {
   strcpy(histPath, getenv("HOME"));
   strcat(histPath, HISTFILE);
}
