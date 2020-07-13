// Simple buffered I/O library interface

#define MyEOF (-1)

typedef struct _myFile {
   int   fd;      // file descriptor
   char  mode;    // mode = 'r' or 'w'
   int   pos;     // index in buffer of next char
   int   nchars;  // number of chars in buffer
   char *buffer;  // dynamically allocated buffer
} MyFile;

// open a file for reading or writing
MyFile *myfOpen(char *path, char mode);

// get next char from open MyFile
int myfGetc(MyFile *fp);

// put char onto MyFile output stream
void myfPutc(MyFile *fp, int ch);

// close an open MyFile
void myfClose(MyFile *fp);