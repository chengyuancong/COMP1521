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
   x.a = 0x0;
   x.b = 0x12;
   x.c = 0x34567;

   printf("%lu\n",sizeof(x));
   char * p = (char *)&x;
   for (int i = 0; i < sizeof(x); i++) {
      printf("%x ", *p);
      p++;
   }
   return 0;
}
