int
bt1_add(BTREE1 * bt, void *op, void *key)
{
   int uniqfound;

   if (bt->memused > bt->limit)
      return 1;
   if (_bt1_search(bt, op, bt->root, key, &uniqfound))
      return 1;
   if (bt->unique && !uniqfound)
      return 0;
   return _bt1_add(bt, bt->cur, bt->curpos, key, 0, NULL, NULL, NULL, NULL);
}
