#include <stdio.h>

//Does not work on all implemetations of linux
int main (int argc, char *argv[], char *envp[]){
    int i = 0 ;
    while(envp[i] != NULL){
        printf("%s\n",envp[i]);
        i++;
    }
    return 0;
}