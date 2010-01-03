typedef struct _RDD_FILTER_
{
   int       handle;
   char      active;
   char      custom;
   char      optimize;
   RDD_FPS  *fps;
   int       nfps;
   char     *sfilter;
   unsigned int *rmap;
   int       size;
   unsigned int *list;
   unsigned int listlen;
   unsigned int cursor;
   int       recno;		/* m6_FiltGoRec(), m6_FiltSkip(), ... */
   RDD_DATA *rd;
} RDD_FILTER;
