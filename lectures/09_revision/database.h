/*
 * Database functionality:
 *
 * usage: ./database
 *
 * The database will accept incoming requests from STDIN. 
 *
 * The database will respond with messages to STDOUT
 *
 * Tip: replace stdin/stdout with the ends of a pipe() call...
 *
 */

#define MAJOR_LENGTH 6
#define MAX_NAME 20

struct record {
    // 5555555...
    int  id;
    // 'Emmet', just the first name.
    char name[MAX_NAME];
    // SENGAH, COMP1A, etc.
    char program[MAJOR_LENGTH];
};

// Updates a record to the database file.
// Creates a new record if it doesn't exist;
//  overwrites the existing one if it does
int write_record(struct record rec);

// Grabs a student record matching a given zid.
// Returns NULL if no such record exists.
struct record * fetch_by_id(int id);
