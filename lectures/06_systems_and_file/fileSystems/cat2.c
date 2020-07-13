// cat2.c ... copy file to stdout

#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>

// guess for max chars in a line
#define MAX 1000

int main(int argc, char *argv[])
{
   // process args and set up for read
   if (argc < 2) 
      error(1, 0, "Usage: %s File", argv[0]);
   FILE *in = fopen(argv[1], "r");
   if (in == NULL)
      error(errno, errno, "Can't open %s", argv[1]);

   // read line-by-line

   char line[MAX+1];
   while (fgets(line,MAX,in) != NULL)
     fputs(line,stdout);

   // release resources
   fclose(in);
   return 0;
}
