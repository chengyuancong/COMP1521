/*
   Lab - Debugging with gdb
   File: words.c
*/

// Include files
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//Constants
#define LINELENGTH    100


// Function prototypes
void reverse_words(char* words[], char* rwords[], int count);
void print_words(char* words[]);
int  mark_words(char* line, char* words[]);


// Main Function
int main( void )
{
  char *line; 
  char *words[51];
  char *rwords[51];
  int   count;

  if(( line = (char*) malloc(LINELENGTH * sizeof(char))) == NULL ) {
    return 1;
  }
  strcpy(line, "this   is a sample  line to      be broken into words.");

  count = mark_words( line, words );
    
  reverse_words( words, rwords, count );

  print_words( rwords );

  return 0;
}

/*
   Converts line to a packed string of words and puts
   pointers to each word in words[].
  
   returns the number of words it found.
*/
int mark_words( char* line, char* words[] )
{
  int i, count = 1;
  char inbetween = 0;

  words[0] = line;
  for( i=0; line[i] != 0 ; ++i ) {
    if( isspace( line[i] )) {
      inbetween = 1;
      line[i] = '\0';
    }
    else { // i.e. line[i] is not a whitespace character
      if( inbetween ) {
	inbetween = 0;
	words[count++] = line + i;
      }
    }
  }
  words[count] = NULL;
  return count;
}

/*
   Copies the pointers in words[] to rwords[] in reverse order.
   count is the number of pointers in words[]
*/
void reverse_words( char* words[], char* rwords[], int count )
{
  int j = 0;

  for( j = 0; words[j] != NULL; j++ ) {
    rwords[j] = words[count-j-1];
  }
  rwords[j] = '\0';
}

/*
   Prints each word in words[] to stdout in order and preceded by its number.
*/
void print_words( char* words[] )
{
  int k;

  for( k=0; words[k] != 1; k++ ) {
    printf("%2d. %s\n", k+1, words[k]);
  }
}
