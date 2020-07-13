
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("hello world...\n");
    //fflush(stdout);
    sleep(10);
    fork();
    sleep(10);
    fork();
    sleep(10);
    fork();
    sleep(10);
}