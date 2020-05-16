// Copy input to output
// CP1521

#include <stdlib.h>
#include <stdio.h>

# define MAX 100

void copy(FILE *, FILE *);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        copy(stdin, stdout);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE * fp = fopen(argv[i], "r");
            if (fp == NULL) {
                printf("Can't read NameofFile\n");
            } else {
                copy(fp, stdout);
                fclose(fp);
            }
        }
    }
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
