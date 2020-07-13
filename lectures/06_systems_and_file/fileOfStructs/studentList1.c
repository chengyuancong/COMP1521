// slist1.c ... show all student records via read()

#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include "Student.h"

// Student = (id,given,family,prog,wam)

int main(void)
{
   int stufd = open("Students1",O_RDONLY);
   if (stufd < 0)
      error(errno, errno, "Can't open student data");

   Student s;
   while(read(stufd,&s,sizeof(Student)) > 0){
      showStudentData(s);
   }

   close(stufd);
   return 0;
}