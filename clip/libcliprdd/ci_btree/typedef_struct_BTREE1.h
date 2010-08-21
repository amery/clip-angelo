typedef struct _BTREE1_
{
   int (*bt_compare) (void *op, void *key1, void *key2, int *uniqfound);
   int keysize;
   int recsize;
   size_t pagesize;
   int keysonpage;
   int halfpage;
   void *root;
   void *cur;
   short curpos;
   int limit;
   int memused;
   int unique;
} BTREE1;
