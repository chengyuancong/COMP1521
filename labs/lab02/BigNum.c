// BigNum.h ... LARGE positive integer values

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "BigNum.h"

// Initialise a BigNum to N bytes, all zero
void initBigNum(BigNum *n, int Nbytes)
{
   n->nbytes = Nbytes;
   n->bytes = malloc(sizeof(Byte) * n->nbytes);
   assert(n->bytes != NULL);
   memset(n->bytes, '0', n->nbytes);
}

// Add two BigNums and store result in a third BigNum
void addBigNums(BigNum n, BigNum m, BigNum *res)
{
   if (n.nbytes > m.nbytes) {
      res->nbytes = n.nbytes + 1;
   } else if (n.nbytes < m.nbytes) {
      res->nbytes = m.nbytes + 1;
   } else {
      res->nbytes = n.nbytes + 1;
   }
   res->bytes = realloc(res->bytes, res->nbytes);
   assert(res->bytes != NULL);
   memset(res->bytes, '0', res->nbytes);
   int i = 0;
   int carry = 0;
   int sum = 0;
   while (i < res->nbytes - 1) {
      sum = 0;
      if (i < n.nbytes) {
         sum += n.bytes[i] - '0';
      }
      if (i < m.nbytes) {
         sum += m.bytes[i] - '0';
      }
      sum += carry;
      if (sum >= 10) {
         res->bytes[i] = (sum - 10) + '0';
         carry = 1;
      } else {
         res->bytes[i] = sum + '0';
         carry = 0;
      }
      i++;
   }
   if (carry == 1) {
      res->bytes[i] = '1';
   }
}

// Subtract two BigNums and store result in a third BigNum
void subtractBigNums(BigNum n, BigNum m, BigNum *res)
{  
   int length_n = 0;
   int length_m = 0;
   int i = n.nbytes - 1;
   while (n.bytes[i] == '0' && i > 0) {
      i--;
   }
   while (i >= 0) {
      length_n++;
      i--;
   }
   i = m.nbytes - 1;
   while (m.bytes[i] == '0' && i > 0) {
      i--;
   }
   while (i >= 0) {
      length_m++;
      i--;
   }
   if (length_n < length_m) {
      BigNum tmp = n;
      n = m;
      m = tmp;
   } else if (length_m == length_n) {
      int k = length_m - 1;
      while (k >= 0) {
         if (n.bytes[k] < m.bytes[k]) {
            BigNum tmp = n;
            n = m;
            m = tmp;
            break;
         }
         k--;
      }
   }
   if (res->nbytes < n.nbytes) {
      res->nbytes = n.nbytes;
      res->bytes = realloc(res->bytes, res->nbytes);
      assert(res->bytes != NULL);
      memset(res->bytes, '0', res->nbytes);
   }
   int j = 0;
   int carry = 0;
   int diff = 0;
   while (j < res->nbytes) {
      diff = 0;
      if (j < n.nbytes) {
         diff += n.bytes[j] - '0';
      }
      if (j < m.nbytes) {
         diff -= m.bytes[j] - '0';
      }
      diff += carry;
      if (diff < 0) {
         res->bytes[j] = (10 + diff) + '0';
         carry = -1;
      } else {
         res->bytes[j] = diff + '0';
         carry = 0;
      }
      j++;
   }
}

// Multiply two BigNums and store result in a third BigNum
void multiplyBigNums(BigNum n, BigNum m, BigNum *res)
{
   int length_n = 0;
   int length_m = 0;
   int i = n.nbytes - 1;
   while (n.bytes[i] == '0' && i > 0) {
      i--;
   }
   while (i >= 0) {
      length_n++;
      i--;
   }
   i = m.nbytes - 1;
   while (m.bytes[i] == '0' && i > 0) {
      i--;
   }
   while (i >= 0) {
      length_m++;
      i--;
   }
   BigNum digit_produtct[length_m];
   for (int k = 0; k < length_m; k++) {
      initBigNum(&digit_produtct[k], n.nbytes + m.nbytes);
   }
   for (int k = 0; k < length_m; k++) {
      int j = 0;
      int carry = 0;
      int product = 0;
      while(j < length_n) {
         product = (n.bytes[j] - '0')* (m.bytes[k] - '0') + carry;
         digit_produtct[k].bytes[k + j] = (product % 10) + '0';
         carry = product / 10;
         j++;
      }
      if (carry > 0) {
         digit_produtct[k].bytes[k + j] = carry + '0';
      }
   }
   if (res->nbytes < n.nbytes + m.nbytes) {
      res->nbytes = n.nbytes + m.nbytes;
      res->bytes = realloc(res->bytes, res->nbytes);
      assert(res->bytes != NULL);
      memset(res->bytes, '0', res->nbytes);
   }
   for (int k = 0; k < length_m; k++) {
      int l = 0;
      int carry = 0;
      int sum = 0;
      while (l < n.nbytes + m.nbytes) {
         sum = (res->bytes[l] - '0') + (digit_produtct[k].bytes[l] - '0') + carry;
         if (sum >= 10) {
            res->bytes[l] = (sum - 10) + '0';
            carry = 1;
         } else {
            res->bytes[l] = sum + '0';
            carry = 0;
         }
         l++;
      }
   }
}

// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum(char *s, BigNum *n)
{  
   int length = strlen(s);
   int k = 0;
   char *cp = s;
   while (isdigit(*cp) == 0 && k < length) {
      cp++;
      k++;
   }
   char *start = cp;
   int num_len = 0;
   while (isdigit(*cp) != 0 && k < length) {
      num_len++;
      cp++;
      k++;
   }
   if (num_len == 0) {
      return 0;
   }
   if (num_len > n->nbytes) {
      n->nbytes = num_len;
      n->bytes = realloc(n->bytes, n->nbytes);
      assert(n->bytes != NULL);
      memset(n->bytes, '0', n->nbytes);
   }
   int i = 0;
   int j = num_len - 1;
   while (i < num_len) {
      n->bytes[i] = start[j];
      i++;
      j--;
   }
   return 1;
}

// Display a BigNum in decimal format
void showBigNum(BigNum n)
{
   int i = n.nbytes - 1;
   while (n.bytes[i] == '0' && i > 0) {
      i--;
   }

   while (i >= 0) {
      printf("%c", n.bytes[i]);
      i--;
   }
}

void freeBigNum(BigNum n){
   free(n.bytes);
}