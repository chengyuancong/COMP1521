// Virtual Memory Simulation

#include <stdlib.h>
#include <stdio.h>

typedef unsigned int uint;

// Page Table Entries

#define NotLoaded 0
#define Loaded    1
#define Modified  2

#define PAGESIZE  4096
#define PAGEBITS  12

#define actionName(A) (((A) == 'R') ? "read from" : "write to")

typedef struct {
   int status;        // Loaded or Modified or NotLoaded
   int frameNo;       // -1 if page not loaded
   int lastAccessed;  // -1 if never accessed
} PTE;

// Globals

uint nPages;         // how many process pages
uint nFrames;        // how many memory frames

PTE *PageTable;      // process page table
int *MemFrames;      // memory (each frame holds page #, or -1 if empty)

uint nLoads = 0;     // how many page loads
uint nSaves = 0;     // how many page writes (after modification)
uint nReplaces = 0;  // how many Page replacements

uint clock = 0;      // clock ticks

// Functions

void initPageTable();
void initMemFrames();
void showState();
int  physicalAddress(uint vAddr, char action);

// main:
// read memory references
// maintain VM data structures
// argv[1] = nPages, argv[2] = nFrames
// stdin contains lines of form
//   R Address
//   W Address
// R = read a byte, W = write a byte, Address = byte address
// Address is mapped to a page reference as per examples in lectures
// Note: pAddr is signed, because -ve used for errors

int main (int argc, char **argv)
{
   char line[100]; // line buffer
   char action;    // read or write
   uint vAddr;     // virtual address (unsigned)
   int  pAddr;     // physical address (signed)

   if (argc < 3) {
      fprintf(stderr, "Usage: %s #pages #frames < refFile\n", argv[0]);
      exit(1);
   }

   nPages = atoi(argv[1]);
   nFrames = atoi(argv[2]);
   // Value 2 also picks up invalid argv[x]
   if (nPages < 1 || nFrames < 1) {
      fprintf(stderr, "Invalid page or frame count\n");
      exit(1);
   }

   initPageTable(); 
   initMemFrames();

   // read from standard input
   while (fgets(line,100,stdin) != NULL) {
      // get next line; check valid (barely)
      if ((sscanf(line, "%c %d\n", &action, &vAddr) != 2)
                     || !(action == 'R' || action == 'W')) {
         printf("Ignoring invalid instruction %s\n", line);
         continue;
      }
      // do address mapping
      pAddr = physicalAddress(vAddr, action);
      if (pAddr < 0) {
         printf("\nInvalid address %d\n", vAddr);
         exit(1);
      }
      // debugging ...
      printf("\n@ t=%d, %s pA=%d (vA=%d)\n",
             clock, actionName(action), pAddr, vAddr);
      // tick clock and show state
      showState();
      clock++;
   }
   printf("\n#loads = %d, #saves = %d, #replacements = %d\n", nLoads, nSaves, nReplaces);
   return 0;
}

// map virtual address to physical address
// handles regular references, page faults and invalid addresses

int physicalAddress(uint vAddr, char action)
{
   int pAddr = 0;
   uint pageno = vAddr / PAGESIZE;
   uint offset = vAddr % PAGESIZE;
   // if the page# is not valid, return -1
   if (pageno >= nPages) {
      return -1;
   }
   // if the page is already loaded
   if (PageTable[pageno].status == Loaded || PageTable[pageno].status == Modified) {
      if (action == 'W') {
         PageTable[pageno].status = Modified;
      }
      PageTable[pageno].lastAccessed = clock;
      pAddr = (PageTable[pageno].frameNo) * PAGESIZE + offset;
   } else {
      // look for an unused frame
      int fno = 0;
      while (fno < nFrames && MemFrames[fno] != -1) {
         fno++;
      }
      // if find one, use that
      if (fno < nFrames) {
         MemFrames[fno] = pageno;
      } else {
         // need to replace a currently loaded frame
         nReplaces++;
         // find the Least Recently Used loaded page
         int least = 0;
         int pno = 0;
         while (pno < nPages && PageTable[pno].status == NotLoaded) {
            pno++;
         }
         least = pno;
         while (pno < nPages) {
            if (PageTable[pno].lastAccessed < PageTable[least].lastAccessed
                && PageTable[pno].status != NotLoaded) {
               least = pno;
            }
            pno++;
         }
         // increment the nSaves counter if modified
         if (PageTable[least].status == Modified) {
            nSaves++;
         }
         fno = PageTable[least].frameNo;
         MemFrames[fno] = pageno;
         // set its PageTable entry to indicate "no longer loaded" 
         PageTable[least].status = NotLoaded;
         PageTable[least].frameNo = -1;   
         PageTable[least].lastAccessed = -1;
      }
      nLoads++;
      // set PageTable entry for the new page
      PageTable[pageno].status = action == 'W'? Modified : Loaded;
      PageTable[pageno].frameNo = fno;
      PageTable[pageno].lastAccessed = clock;
      pAddr = (PageTable[pageno].frameNo) * PAGESIZE + offset;
   }
   return pAddr;
}

// allocate and initialise Page Table

void initPageTable()
{
   PageTable = malloc(nPages * sizeof(PTE));
   if (PageTable == NULL) {
      fprintf(stderr, "Insufficient memory for Page Table\n");
      exit(1);
   }
   for (int i = 0; i < nPages; i++) {
      PageTable[i].status = NotLoaded;
      PageTable[i].frameNo = -1;
      PageTable[i].lastAccessed = -1;
   }
}

// allocate and initialise Memory Frames

void initMemFrames()
{
   MemFrames = malloc(nFrames * sizeof(int));
   if (MemFrames == NULL) {
      fprintf(stderr, "Insufficient memory for Memory Frames\n");
      exit(1);
   }
   for (int i = 0; i < nFrames; i++) {
      MemFrames[i] = -1;
   }
}

// dump contents of PageTable and MemFrames

void showState()
{
   printf("\nPageTable (Stat,Acc,Frame)\n");
   for (int pno = 0; pno < nPages; pno++) {
      uint s; char stat;
      s = PageTable[pno].status;
      if (s == NotLoaded)
         stat = '-';
      else if (s & Modified)
         stat = 'M';
      else
         stat = 'L';
      int f = PageTable[pno].frameNo;
      printf("[%2d] %2c, %2d, %2d",
             pno, stat, PageTable[pno].lastAccessed, PageTable[pno].frameNo);
      if (f >= 0) printf(" @ %d", f*PAGESIZE);
      printf("\n");
   }
   printf("MemFrames\n");
   for (int fno = 0; fno < nFrames; fno++) {
      printf("[%2d] %2d @ %d\n", fno, MemFrames[fno], fno*PAGESIZE);
   }
}
