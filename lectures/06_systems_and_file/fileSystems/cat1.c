// cat1.c ... copy file to stdout

#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  
   // process args and set up for read
   if (argc < 2) 
      error(1, 0, "Usage: %s File", argv[0]);
   FILE *in = fopen(argv[1], "r");
   setbuf(in,NULL);
   setbuf(stdout,NULL);
   if (in == NULL)
      error(errno, errno, "Can't open %s", argv[1]);

   // read char-by-char

   int ch;
   while ((ch = fgetc(in)) != EOF)
      putchar(ch);

   // release resources
   fclose(in);
   return 0;
}
