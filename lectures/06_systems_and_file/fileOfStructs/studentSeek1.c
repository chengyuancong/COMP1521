// sstu1.c ... show the i'th student

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
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

   int stufd = open("Students1",O_RDONLY);
   if (stufd < 0)
      error(errno, errno, "Can't open student data");

   //Seek to byte offset: 
   int offset = sizeof(Student) * i;
   if (lseek(stufd,offset,SEEK_SET) < 0)
		error(errno, errno, "Can't seek to %d'th", i);

   Student s;
   if (read(stufd,&s,sizeof(Student)) < sizeof(Student))
      error(1, errno, "Can't read %d'th", i);

   showStudentData(s);

   close(stufd);
   return 0;
}