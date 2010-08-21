typedef struct _RDD_MEMO_
{
   char *name;
   char *path;
   RDD_FILE file;
   int format;
   int blocksize;
   DbfLocale *loc;
   struct _RDD_MEMO_VTBL_ *vtbl;
   int updated;
   int flex_nremoved;
   unsigned int *flex_removed;
} RDD_MEMO;
