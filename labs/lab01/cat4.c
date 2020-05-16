// Copy input to output
// CP1521

#include <stdlib.h>
#include <stdio.h>

void copy(FILE *, FILE *);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        copy(stdin, stdout);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE * fp = fopen(argv[i], "r");
            if (fp == NULL) {
                fprintf(stderr, "Can't read %s\n", argv[i]);
            } else {
                copy(fp, stdout);
                fclose(fp);
            }
        }
    }
    return EXIT_SUCCESS;
}

// Copy contents of input to output
// Assumes both files open in appropriate mode

void copy(FILE *input, FILE *output)
{
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, input) != NULL) {
        fputs(buf, output);
    }
}
