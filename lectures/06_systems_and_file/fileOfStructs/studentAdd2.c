// sadd2.c ... add a student record via printf()

#include <stdio.h>
#include <error.h>
#include <errno.h>
#include "Student.h"

// Student = (id,given,family,prog,wam)

int main(void)
{
   FILE *stuf = fopen("Students2","a");
   if (stuf == NULL)
      error(errno, errno, "Can't open student data");

   Student  s;
   if(getStudentData(&s) < 0 ){
	  error(1, 0, "Student data format error");
   } 
   showStudentData(s);

   fprintf(stuf,"%d:%s:%s:%d:%f\n",s.id,s.given,s.family,s.prog,s.wam);

   fclose(stuf);

   return 0;
}