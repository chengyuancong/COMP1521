// Copy input to output
// CP1521

#include <stdlib.h>
#include <stdio.h>

# define MAX 100

void copy(FILE *, FILE *);

int main(int argc, char *argv[])
{
	copy(stdin,stdout);
	return EXIT_SUCCESS;
}

// Copy contents of input to output
// Assumes both files open in appropriate mode

void copy(FILE *input, FILE *output)
{
    char buf[MAX];
    while (fgets(buf, MAX, input) != NULL) {
        fputs(buf, output);
    }
}
