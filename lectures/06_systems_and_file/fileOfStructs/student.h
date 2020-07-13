// Simple Student Data Type

#define MAXNAME 50

typedef struct _student {
   int   id;              // zID
   char  given[MAXNAME];  // given name
   char  family[MAXNAME]; // family name
   int   prog;            // enrolled program
   float wam;             // weighted avg mark
} Student;

int getStudentData(Student *);
void showStudentData(Student);