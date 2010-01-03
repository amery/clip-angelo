typedef struct _RDD_FILE_
{
   int       fd;
   caddr_t   md;
   size_t    mapsize;
   char     *dbf;
   long      filehash;
   int       rlocked;
   int       wlocked;
   int       f64;
} RDD_FILE;
