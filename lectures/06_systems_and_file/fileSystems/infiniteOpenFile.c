#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

int main(void){

   FILE *in;  int ch;
   while (1) {
      if ((in = fopen("abc", "r")) == NULL){
         error(1,errno,NULL);
      }
      while ((ch = fgetc(in)) != EOF)
         putc(ch, stdout);
	 //If we uncomment this, it will run infinitely
	 //fclose(in);
   }
   return 0;
}