// Multiply two numbers (numbers can be LARGE)

#include <stdio.h>
#include "BigNum.h"

int main(int argc, char **argv)
{
   BigNum num1;  // first input number
   BigNum num2;  // second input number
   BigNum product;   // num1 * num2

   if (argc < 3) {
      printf("Usage: %s Num1 Num2\n", argv[0]);
      return 1;
   }

   // Initialise BigNum objects
   initBigNum(&num1,     20);
   initBigNum(&num2,     20);
   initBigNum(&product,  20);

   // Extract values from cmd line args
   if (!scanBigNum(argv[1], &num1)) {
      printf("First number invalid\n");
      return 1;
   }
   if (!scanBigNum(argv[2], &num2)) {
      printf("Second number invalid\n");
      return 1;
   }

   // Multiply num1*num2, store result in product
   multiplyBigNums(num1, num2, &product);

   printf("Product of "); showBigNum(num1);
   printf("\nand "); showBigNum(num2);
   printf("\nis "); showBigNum(product);
   printf("\n");
   freeBigNum(num1);
   freeBigNum(num2);
   freeBigNum(product);
   return 0;

}
