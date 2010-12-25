struct _HashTable
{
   int size;
   int num;
   int current;
   unsigned long *hashs;
   char *status;
   void **items;
};
