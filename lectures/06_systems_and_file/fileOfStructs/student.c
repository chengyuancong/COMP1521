// Functions on Student structs

#include <stdio.h>
#include <string.h>
#include "Student.h"

#define MAXLINE 100

int getStudentData(Student *s)
{
   int  nerrs = 0;
   char resp[MAXLINE];

   printf("ID          : ");
   fgets(resp, MAXLINE, stdin);
   if (sscanf(resp, "%d", &(s->id)) != 1) nerrs++;
   printf("Given name  : ");
   fgets(s->given, MAXNAME, stdin);
   s->given[strlen(s->given)-1] = '\0';
   printf("Family name : ");
   fgets(&(s->family[0]), MAXNAME, stdin);
   s->family[strlen(s->family)-1] = '\0';
   printf("Program     : ");
   fgets(resp, MAXLINE, stdin);
   if (sscanf(resp, "%d", &(s->prog)) != 1) nerrs++;
   printf("WAM         : ");
   fgets(resp, MAXLINE, stdin);
   if (sscanf(resp, "%f", &(s->wam)) != 1) nerrs++;

   return (nerrs > 0) ? -1 : 0;
}

void showStudentData(Student s)
{
   printf("%s %s (%d), studying %d, with wam %0.1f\n",
          s.family, s.given, s.id, s.prog, s.wam);
}