#include <stdio.h>
#include <stdlib.h>
int main(void) {
    char *value = getenv("HOME");
    printf("Environment variable 'HOME' has value '%s'\n", value);
    return 0;
}