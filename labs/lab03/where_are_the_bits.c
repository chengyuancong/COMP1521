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

union _bits32 {
   uint32_t xval;
   struct _bit_fields bits;
};

int main(void)
{
   struct _bit_fields x;
   x.a = 0b1;
   x.b = 0b1;
   x.c = 0b1;

   union _bits32 u;
   u.bits = x;

   printf("Size of x is %lu bytes.\n",sizeof(x));
   
   printf("\nc: 00000000000000000001 b: 00000001 a: 0001\n");

   printf("\nPrint abc by uint32_t:\n");
   for (int i = 0; i < 32; i++) {
      printf("%d", (u.xval >> (31 - i)) & 1);
   }

   printf("\n\nPrint abc by bytes:\n");
   char * p = (char *) &x;
   for (int i = 0; i < sizeof(x); i++) {
      for (int j = 7; j >= 0; j--) {
         printf("%d", (*p >> j) & 1);
      }
      printf(" ");
      p++;
   }
   printf("\n");

   return 0;
}
