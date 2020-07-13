#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

int main(void){

   FILE *in;  
   int ch;
    
   if ((in = fopen("abc", "r")) == NULL){
         error(1,errno,NULL);
   }
   
   while ((ch = fgetc(in)) != EOF)
      putc(ch,stdout); //Could just use putchar(ch);
   fclose(in);
   return 0;
}