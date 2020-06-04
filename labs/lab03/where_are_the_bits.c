// where_are_the_bits.c ... determine bit-field order
// CP1521 Lab 03 Exercise
// Written by Yuancong

#include <stdio.h>
#include <stdlib.h>

struct _bit_fields {
   unsigned int a : 4,
                b : 8,
                c : 20;
};

int main(void)
{
   struct _bit_fields x;
   x.a = 1;
   x.b = 1;
   x.c = 1;

   printf("\nSize of x is %lu bytes.\n", sizeof(x));

   printf("\nPrint abc as an int:\n");
   int * ip = (int *) &x;
   for (int k = 31; k >= 0; k--) {
      printf("%d", (*ip >> k) & 1);
   }

   printf("\n\nPrint abc by bytes:\n");
   char * cp = (char *) &x;
   for (int i = 0; i < sizeof(x); i++) {
      for (int j = 7; j >= 0; j--) {
         printf("%d", (*cp >> j) & 1);
      }
      printf(" ");
      cp++;
   }
   printf("\n");
   return 0;
}

