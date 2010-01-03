typedef struct _RDD_DATA_
{
   char     *name;
   char     *path;

   int       area;
   int       rdhandle;
   int       sig;

   struct _RDD_DATA_VTBL_ *vtbl;

   DbfLocale *loc;

   RDD_ORDER **orders;
   int       curord;
   int       ords_opened;

   RDD_INDEX **indices;
   int       idxs_opened;

   struct _RDD_RELATION_ **relations;
   int       rels_opened;
   struct _RDD_RELATION_ **parents;
   int       pars_opened;
   struct _RDD_DATA_ *pending_child_parent;

   RDD_MEMO *memo;

   RDD_FILE  file;

   RDD_FIELD *fields;
   int       nfields;
   HashTable *hashes;
   int       nulloffs;

   struct _RDD_FILTER_ *filter;
   char     *yylval;
   char     *ptr;
   char     *word;
   int       wlen;
   int       curlex;
   char     *rmfilter;
   int       rmflen;

   int       hdrsize;
   int       recsize;

   char      shared;
   char      readonly;
   char      tempo;

   char      bof;
   char      eof;
   char      v_bof;
   unsigned int recno;

   char      newrec;
   char      flocked;

   unsigned int *locks;
   int       nlocks;

   RDD_ORDSTATE os;

   ClipVar  *locate_wpar;
   int       locate_next;
   struct _RDD_FILTER_ *locate_filter;

   char      updated;

   char     *indexing;
   char     *tagindexing;
   int       reindexing;
   unsigned int keysincluded;

   char      lockstyle;
   char      locked;

   ClipVar **data;
   void     *record;
   char      valid;
   char      changed;		/* record */
   char      x;			/* 'X' field type */
   ClipMachine *ClipMachineMemory;
} RDD_DATA;
