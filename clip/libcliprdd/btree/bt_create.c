BTREE *
bt_create(int unique, int count, int size, int (*compare) (void *op, void *key1, void *key2, int *uniqfound))
{
   BTREE *bt = (BTREE *) calloc(1, sizeof(BTREE));

   bt->unique = unique;
   bt->bt_compare = compare;
   bt->size = size;
   bt->count = count;
   bt->in = 0;

   if (bt->data == NULL)
   {
      bt->data = calloc(count + 1, (size + sizeof(BT_NODE)));
   }

   if (!bt->data)
   {
      free(bt);
      bt = NULL;
   }
   return bt;
}
