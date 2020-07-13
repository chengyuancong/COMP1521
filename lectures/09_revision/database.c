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
     */

    
    FILE * file_ptr;

    if(argc > 1 && strcmp(argv[1],"-new") == 0){
        // Create database if it doesn't exist, emptying it
        file_ptr = fopen(DATABASE_FILE, "w");
    } else {
         // Create database if it doesn't exist, without emptying it
        file_ptr = fopen(DATABASE_FILE, "a");
    }
    if(file_ptr == NULL){
        error(errno,errno,"Could not open file %s",DATABASE_FILE);
    }
    fclose(file_ptr);
    
    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, stdin)) {
        int id;
        struct record rec;
        if ( buf[0] == 'f'){
            if( sscanf(buf, "f %d", &id) != 1){
                printf("fail\n");
            } else {
                struct record * res = fetch_by_id(id);
                if (res == NULL) {
                    printf("fail\n");
                } else {
                    printf("%d %s %s\n", res->id, res->name, res->program);
                    free(res);
                }
            }    
        } else if (buf[0] == 'w'){
            if(sscanf(buf, "w %d %s %s", &rec.id, rec.name, rec.program) != 3){
                printf("fail\n");
            } else {
                if(write_record(rec) < 0){
                    printf("fail\n");
                } else {
                    printf("success\n");
                }
            }

        } else {
            printf("fail\n");
        }
    } 

    return 0;
}

struct record * fetch_by_id(int id) {
    int fd = open(DATABASE_FILE, O_RDONLY);
    if (fd < 0) {
        error(errno,errno,"Could not open file %s: ",DATABASE_FILE);
    }

    // Scan through records in DB
    struct record * curr = malloc(sizeof(struct record));
    if( curr == NULL){
        fprintf(stderr,"Malloc error\n");
        exit(1);
    }
    while (read(fd, curr, sizeof(struct record)) > 0) {
        if (curr->id == id) {
            close(fd);
            return curr;
        }
    }

    // Failed to find
    free(curr);
    close(fd);
    return NULL;
}


int write_record(struct record rec) {
    // Open database
    int fd = open(DATABASE_FILE, O_RDWR);
    if (fd < 0) {
        error(errno,errno,"Could not open file %s: ",DATABASE_FILE);
    }

    struct record curr;
    while (read(fd, &curr, sizeof(struct record)) > 0) {
        if (curr.id == rec.id) {
            int check = lseek(fd, -1*(sizeof(struct record)), SEEK_CUR);
            if(check < 0){
                close(fd);
                return -1;
            }
            break;  
        }
    }
    if(write(fd, &rec, sizeof(struct record)) < 0){           
        close(fd);
        return -1;
    }
    close(fd);
    return 1;
}
