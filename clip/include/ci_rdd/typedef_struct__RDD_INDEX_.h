typedef struct _RDD_INDEX_
{
   char *name;
   char *path;
   int indexno;
   int structural;
   RDD_FILE file;
   RDD_ORDER **orders;
   int norders;
   DbfLocale *loc;
   struct _RDD_INDEX_VTBL_ *vtbl;
   struct _RDD_DATA_ *rd;
   int updated;
} RDD_INDEX;
