typedef struct ClipMachine
{
   struct ClipMachine *next;
   int       inited;
   int       deleted;

   ClipVar  *bp;
   ClipFrame *fp;
   int       argc;

   ClipVar  *trapVar;
   ClipVect  trapTrace;

   struct _HashTable *publics;
   struct _HashTable *privates;
   struct _HashTable *spaces;

   struct _HashTable *functions;
   int       main_func_level;
   ClipVect  dlls;
   ClipBlock *exits;
   int       nexits;
   ClipFunction **cexits;
   int       ncexits;
   struct _HashTable *keys;

   int       corba;

   DBDriver **dbdrivers;
   int      *ndbdrivers;
   struct _RDD_DATA_VTBL_ ***data_drivers;
   int      *ndata_drivers;
   struct _RDD_INDEX_VTBL_ ***idx_drivers;
   int      *nidx_drivers;
   struct _RDD_MEMO_VTBL_ ***memo_drivers;
   int      *nmemo_drivers;
   char     *def_data_driver;
   char     *def_idx_driver;
   char     *def_memo_driver;
   char     *def_db_driver;

   SQLDriver **sqldrivers;
   int      *nsqldrivers;

   struct _HashTable *tasklocks;
   struct _HashTable *fileopens;

   int       neterr;

   struct _HashTable *aliases;
   ClipVect *areas;
   ClipVect *areaStack;
   long      curArea;

   void     *driver;

  /* Visual FoxPro's DATETIME format */
   int       hours;
   int       seconds;

   unsigned long flags;
   unsigned long flags1;
   int       optimizelevel;
   int       m6_error;
   int       index_buffer_limit;
   int       lockstyle;
   int       mblocksize;
   int       autorder;
   char     *date_format;
   int       decimals;
   int       fileCreateMode;
   int       dirCreateMode;
   int       epoch;
   char     *path;
   char     *defaul;
   int       typeahead;
   int      *kbdbuf;
   int     **kbdptr;
   int       lastkey;
   int       cursor;
   char     *altfile;
   void     *out;		/* std out file */
   void     *alt;
   char     *extrafile;
   void     *extra;
   char     *prfile;
   char     *real_prfile;
   void     *printer;
   char     *rootpath;
   int       margin;
   char     *delimchars;
   int       msgline;
   int       notcancel;
   int       eventmask;

   struct Screen *screen;
   struct ScreenBase *screen_base;
   int       fullscreen;
   int       update;
   int       inkey;
   ClipAttr  attr;
   int       colorSelect;	/* for colorselect function */
   int       prow;
   int       pcol;
   char     *buf;
   int       buflen;

   ClipVect  history;

   struct _HashTable *store;
   ClipVar  *errorblock;
   int       errorlevel;
   int       noerrblock;	/* don't call errorblock on errors */
   char      syserr[SYSERRLEN];

   ClipWindow *windows;
   int       wnum;
   ClipRect  wboard;
   int       wshadow;

   Container *container;
   ClipFrame *inMacro;

   struct _HashTable *hashnames;
   int       print_deep;
   int       print_max_deep;

   unsigned char prntbl[256];
   unsigned char prn_xlat[256];
   unsigned char term_xlat[256];

   struct _HashTable *fields;

  /* if not null, all output come here */
   struct OutBuf *obuf;
   struct OutBuf *ebuf;

  /* output to membuf (SET PRINTER TO MEMBUF) */
   struct OutBuf *pbuf;

   ClipVar  *obj;
   struct _HashTable *profiler;
   ProfileBucket *pbucket;
}
ClipMachine;
