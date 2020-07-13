#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    struct stat buf;
    if(argc < 2){
        fprintf(stderr,"Incorrect Usage\n");
        exit(1);
    }	
    if(stat(argv[1],&buf) < 0){
        error(errno,errno,NULL);
    }
    printf("%ld %d %ld %ld\n",buf.st_ino,buf.st_size, 
                              buf.st_blocks,buf.st_blocks*512);
    //This will behave differently with a file that is a soft link                          
    if(lstat(argv[1],&buf) < 0){
        error(errno,errno,NULL);
    }
    printf("%ld %d %ld %ld\n",buf.st_ino,buf.st_size, 
                              buf.st_blocks,buf.st_blocks*512);                           

    

    return 0;
}
