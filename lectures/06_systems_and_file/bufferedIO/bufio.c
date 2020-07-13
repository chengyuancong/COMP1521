// Simple buffered I/O Library
// We only need stdio.h for error messages
// This library is intended to replace stdio

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "bufio.h"

#define MyBUFSIZE 8192

// open a file for reading or writing
MyFile *myfOpen(char *path, char mode)
{
   MyFile *new;
   int o_mode;

   switch (mode) {
   case 'r': o_mode = O_RDONLY; break;
   case 'w': o_mode = O_WRONLY|O_CREAT|O_TRUNC; break;
   default:  return NULL; break; // crude error handling
   }
   if ((new = malloc(sizeof(MyFile))) == NULL)
      return NULL;
   if ((new->buffer = malloc(MyBUFSIZE)) == NULL) {
      free(new);
      return NULL;
   }
   // initialise MyFile
   new->mode = mode;
   new->pos = 0;
   new->nchars = 0;
   new->fd = open(path, o_mode, 0666);
   if (new->fd < 0) {
      free(new->buffer);
      free(new);
   }
   return new;
}

// get next char from open MyFile
int myfGetc(MyFile *fp)
{
   if (fp->mode != 'r') {
      printf("Attempt to read from a non-reading file\n");
      exit(1); // crude error handling
   }
   if (fp->pos == fp->nchars) {
      // reached end of current buffer
      fp->nchars = read(fp->fd, fp->buffer, MyBUFSIZE);
      fp->pos = 0; // scan buffer from start, again
      if (fp->nchars == 0) return MyEOF;
   }
   // get next char from buffer
   int next = fp->buffer[fp->pos];
   fp->pos++;
   return next;
}

// put char onto MyFile output stream
void myfPutc(MyFile *fp, int ch)
{
   if (fp->mode != 'w') {
      printf("Attempt to write to a non-writing file\n");
      exit(1); // crude error handling
   }
   if (fp->nchars == MyBUFSIZE) {
      // buffer is full; flush it
      write(fp->fd, fp->buffer, MyBUFSIZE);
      fp->nchars = 0;
      fp->pos = 0;
   }
   fp->buffer[fp->pos] = ch;
   fp->pos++;
   fp->nchars++;
}

// close an open MyFile
void myfClose(MyFile *fp)
{
   // flush buffer
   if (fp->mode != 'w') {
       write(fp->fd, fp->buffer, fp->nchars);
   }
   close(fp->fd);
   free(fp->buffer);
   free(fp);
}