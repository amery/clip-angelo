int
bt1_last(BTREE1 * bt)
{
   if (!NKEYS(bt->root))
      return 1;
   return _bt1_last(bt, bt->root);
}
