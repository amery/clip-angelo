int
bt1_first(BTREE1 * bt)
{
   if (!bt->root || !NKEYS(bt->root))
      return 1;
   return _bt1_first(bt, bt->root);
}
