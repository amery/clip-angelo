typedef struct _BTREE_
{
   int (*bt_compare) (void *op, void *key1, void *key2, int *uniqfound);
   int unique;
   int size;
   unsigned int count;
   int in;
   char type;
   unsigned int root;
   unsigned int cur;
   unsigned int fuu;
   void *data;
} BTREE;
