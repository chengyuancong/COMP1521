#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>

int main(int argc, char *argv[])
{
   if (argc < 2) {
      fprintf(stderr, "Usage: %s File\n", argv[0]);
      exit(1);
   }
    
   int in;
  
   in = open(argv[1], O_RDONLY);
     
   if (in < 0) {
      fprintf(stderr,"Can't open file %s (err:%d)\n", argv[1], errno);
      //exit(errno);
      perror("Can't open file");
      //exit(errno);
      error(errno,errno,"Can't open file %s",argv[1]);   
   }

   close(in);
   return 0;
}