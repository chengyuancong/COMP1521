#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

int main(void){

   FILE *out; 
   //Try changing w to a
   if ((out = fopen("abc", "w")) == NULL){
         error(1,errno,NULL);
   }
   fprintf(out,"3 piece feed\n");
   fprintf(out,"zinger burger\n");
	 
   fclose(out);
   return 0;
}