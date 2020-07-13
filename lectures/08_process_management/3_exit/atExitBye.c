#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void bye(void)
{
   printf("Bye!\n");
}

void byeAgain(void){
   printf("I'll miss you!\n");
}

int main(int argc, char * argv[])
{

   if (atexit(byeAgain) != 0) {
      fprintf(stderr, "cannot set exit function\n");
      exit(EXIT_FAILURE);
   }

   if (atexit(bye) != 0) {
      fprintf(stderr, "cannot set exit function\n");
      exit(EXIT_FAILURE);
   }
 
   //int *x = NULL;

   //printf("%d\n",*x); 	

   FILE *in;
   if ((in = fopen(argv[1],"r")) == NULL) {  
      perror(argv[1]);
      exit(EXIT_FAILURE);
	  //_exit(EXIT_FAILURE);
   }

   char line[100];
   for (int i = 0; i < 5; i++) {
      fgets(line, 100, in);
      fputs(line, stdout);
   }

   fclose(in);
   return 0;  //exit
}
