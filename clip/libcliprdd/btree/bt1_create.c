BTREE1 *
bt1_create(int unique, int keysize, int (*compare) (void *op, void *key1, void *key2, int *uniqfound), int limit)
{
   BTREE1 *bt;

   if (!limit)
      return NULL;

   bt = calloc(1, sizeof(BTREE1));

   bt->bt_compare = compare;
   bt->keysize = keysize;
   bt->recsize = keysize + sizeof(void *);
   bt->pagesize = getpagesize();
   bt->keysonpage = (bt->pagesize - 2 * sizeof(short) - sizeof(void *)) / (bt->recsize + sizeof(short));
   bt->halfpage = bt->keysonpage / 2;
   bt->root = _bt_newpage(bt);
   bt->cur = bt->root;
   bt->curpos = 0;
   bt->limit = (limit * 1024 * 1024) / bt->pagesize;
   bt->unique = unique;
   return bt;
}
