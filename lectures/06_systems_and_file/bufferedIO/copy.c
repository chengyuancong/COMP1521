// Client for buffered I/O library

#include <stdlib.h>
#include "bufio.h"

// can't avoid ... error messages
#include <stdio.h>

int main(int argc, char *argv[])
{
   if (argc < 3) {
      printf("Usage: %s InFile OutFile\n", argv[0]);
      exit(1);
   }
   MyFile *inf;
   if ((inf = myfOpen(argv[1],'r')) == NULL) {
      printf("Can't open %s\n", argv[1]);
      exit(1);
   }
   MyFile *outf;
   if ((outf = myfOpen(argv[2],'w')) == NULL) {
      printf("Can't open %s\n", argv[2]);
      exit(1);
   }
   int ch;
   while ((ch = myfGetc(inf)) != MyEOF)
      myfPutc(outf, ch);
   myfClose(inf);
   myfClose(outf);
   return 0;
}