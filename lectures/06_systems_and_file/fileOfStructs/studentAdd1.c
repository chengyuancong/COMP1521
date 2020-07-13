// sadd1.c ... add a student record via write()

#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>
#include "Student.h"

// Student = (id,given,family,prog,wam)

int main(void)
{

   int stufd = open("Students1",O_WRONLY|O_CREAT|O_APPEND,0666);
   if (stufd < 0)
      error(errno, errno, "Can't open student data");

   Student  s;
   if(getStudentData(&s) != 0){
	  error(1, 0, "Student data format error");
   } 
   showStudentData(s);
   
   ssize_t bytesWritten = write(stufd,&s,sizeof(Student));
   if(bytesWritten < 0)
        error(errno,errno, "Write failed");
   if(bytesWritten != sizeof(Student)){
		error(errno,errno,"Only wrote %ld",bytesWritten);
   }
   
   close(stufd);

   return 0;
}