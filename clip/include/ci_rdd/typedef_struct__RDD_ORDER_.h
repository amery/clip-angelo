typedef struct _RDD_ORDER_
{
   char     *name;
   int       orderno;
   char     *expr;
   unsigned int header;
   int       simpexpr;		/* Simple expression (just a fieldname) */
   int       simpfno;		/* No of key field (for simple expressions) */
   ClipVar   block;		/* Compiled key expression */

   char      type;
   int       unique;
   int       descend;
   int       custom;
   int       canadd;
   int       keysize;
   int       bufsize;
   int       dec;
   short int fullpage;
   short int halfpage;

   struct _RDD_INDEX_ *index;
   struct _RDD_INDEX_VTBL_ *vtbl;

   char     *scopetop;
   int       stoplen;
   char     *scopebottom;
   int       sbotlen;
   ClipVar   scopetopvar;
   ClipVar   scopebottomvar;

   char     *cforexpr;
   ClipVar   bforexpr;

   unsigned int keyno;
   char     *key;

   RDD_STACK stack[MAX_BTREE_DEEP];
   void     *curpage;
   unsigned int curoffs;
   int       level;
   int       valid_stack;
   unsigned short cnt;
   unsigned int cntcdx;

   int       binary;
   int       wlocked;
   int       ic;		/* ignore case */
  /* Independed indices */
   char     *iikey;
   unsigned char id[12];
   int       bof;
   int       eof;
   int       c_item;
   unsigned int rootoffs;
   unsigned short newcnt;
} RDD_ORDER;
