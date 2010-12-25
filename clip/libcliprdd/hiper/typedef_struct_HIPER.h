typedef struct _HIPER_
{
   char *path;
   char *name;
   RDD_FILE file;
   int pagesize;
   int intsonpage;
   int lcase;
   int filtset;
   unsigned int lastrec;
   int shared;
   char seen[0x0ffff];
   unsigned int *map;
   unsigned int *rmap;
   unsigned int recno;
} HIPER;
