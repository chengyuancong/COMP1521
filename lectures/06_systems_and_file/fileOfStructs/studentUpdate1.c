// sstu2.c ... update the i'th student

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "Student.h"

// Student = (id,given,family,prog,wam)

int main(int argc, char *argv[])
{
   if (argc < 2) {
      fprintf(stderr,"Usage: %s N\n", argv[0]);
      exit(1);
   }
   int i;
   if (sscanf(argv[1], "%d", &i) < 1) {
      fprintf(stderr,"Usage: %s N\n", argv[0]);
      exit(1);
   }

   int stufd = open("Students1",O_RDWR);
   if (stufd < 0) {
      perror("Can't open student data");
      exit(errno);
   }
   int status = lseek(stufd, i*sizeof(Student), SEEK_SET);
   if (status < 0) {
      fprintf(stderr, "Can't seek to %d'th\n", i);
      exit(errno);
   }
   Student stude;
   int nbytes = read(stufd, &stude, sizeof(Student));
   if (nbytes < sizeof(Student)) {
      fprintf(stderr, "Can't read %d'th\n", i);
      exit(errno);
   }
   showStudentData(stude);

   printf("New values ...\n");
   if (getStudentData(&stude) < 0) {
      perror("Invalid student data");
      exit(1);
   }
   lseek(stufd, i*sizeof(Student), SEEK_SET);
   nbytes = write(stufd, &stude, sizeof(Student));
   if (nbytes < sizeof(Student)) {
      fprintf(stderr, "Can't update student #%d\n", i);
      exit(errno);
   }

   close(stufd);
   return 0;
}