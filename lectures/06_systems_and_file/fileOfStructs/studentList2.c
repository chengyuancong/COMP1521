// slist2.c ... list student records via scanf()

#include <stdio.h>
#include <error.h>
#include <errno.h>
#include "Student.h"

// Student = (id,given,family,prog,wam)

int main(void)
{
   FILE *stuf = fopen("Students2","r");
   if (stuf == NULL)
      error(errno, errno, "Can't open student data");

   Student s;
   char buffer[200];
   while(fgets(buffer,200,stuf) != NULL){
       sscanf(buffer,"%d:%[^:]:%[^:]:%d:%f",&(s.id),
											s.given,
											s.family,
											&(s.prog),
											&(s.wam));
	    showStudentData(s);
   }

   fclose(stuf);
   return 0;
}