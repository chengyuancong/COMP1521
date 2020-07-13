#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include "database.h"

#define DATABASE_FILE "studentdata.db"

int main (int argc, char * argv[]) {

    /*
     * To interact with the database:
     *
     * w id name program 
     *  - w(rites) record to database
     *  - Prints "success\n" on success, "fail\n" on any kind failure
     * f id              
     *  - f(etches) record with id from database
     *  - prints id, name and program followed by newline on success
     *  - prints "fail\n" on any sort of failure
     *
     */

    
    int fd;
    //Create database if it does not exist.
    if ( argc > 1 && strcmp(argv[1],"-new") == 0){
        fd = open(DATABASE_FILE,O_WRONLY|O_CREAT|O_TRUNC,0666);
    
        if(fd < 0){
            error(errno, errno,"Could not open %s: ", DATABASE_FILE);
        } else {
            close(fd);
        }
    }
     
    
    char buf[BUFSIZ];
    
    while (fgets(buf, BUFSIZ, stdin)) {
        int id;
        if(buf[0] == 'f'){
            struct record * rec;
            if( sscanf(buf,"f %d",&id) != 1){
                printf("fail\n");
            } else {
                struct record * res = fetch_by_id(id);
                if(res == NULL){
                    printf("fail\n");
                
                } else {
                    printf("%d %s %s\n",res->id,res->name,res->program);    
                }
            }
        } else if (buf[0] == 'w'){
            struct record r;
            if( sscanf(buf,"w %d %s %s",&(r.id),r.name,r.program) != 3){
                printf("fail\n");
            } else {
                int ret = write_record(r);
                if(ret == -1){
                    printf("Fail\n");
                } else {
                    printf("Success\n");
                }
            }
        } else {
            printf("fail\n");
        } 
    }

    return 0;
}

struct record * fetch_by_id(int id) {
    int fd = open(DATABASE_FILE,O_RDONLY);
    if(fd < 0) error(errno,errno,"");
    struct record * r = malloc(sizeof(struct record)) ;
    while(read(fd,r,sizeof(struct record)) > 0){
        if(r->id == id){
            close(fd);
            return r;
        }
    }
    close(fd);
    return NULL;
}


int write_record(struct record rec) {
     int fd = open(DATABASE_FILE,O_RDWR);
     if(fd == -1){
         return -1;
     }

     struct record r;
     while(read(fd,&r,sizeof(struct record)) > 0){
         if(r.id == rec.id){
              lseek(fd,-1*(sizeof(struct record)),SEEK_CUR);
              write(fd,&rec,sizeof(struct record));
              close(fd);
              return 1;
         }

     } 
     write(fd,&rec,sizeof(struct record));
     
     close(fd);
     return 1;
}
