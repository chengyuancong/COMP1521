// sstu1.c ... show the i'th student

#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include "Student.h"

// Student = (id,given,family,prog,wam)

int main(int argc, char *argv[])
{
   if (argc < 2)
      error(1, 0, "Usage: %s N", argv[0]);
   int i;
   if (sscanf(argv[1], "%d", &i) < 1)
      error(1, 0, "Usage: %s N", argv[0]);

   FILE * stuf = fopen("Students2","r");
   if (stuf < 0)
      error(errno, errno, "Can't open student data");

   int counter = 0;
   char line[100];
   while(1){
       if(fgets(line,100,stuf) ==NULL){
           counter = -1;
           break;
       }
       if(counter == i) break;
       counter++;
   }
   if(counter == -1){
       fprintf(stderr,"Can't find student #%d\n",i);
       exit(1);
   } else {

	   Student stude;
	   sscanf(line,"%d:%[^:]:%[^:]:%d:%f", 
		      &stude.id,stude.given,stude.family, 
		      &stude.prog, & stude.wam);
	   showStudentData(stude);
   }
   fclose(stuf);
   return 0;
}