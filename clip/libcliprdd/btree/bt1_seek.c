int
bt1_seek(BTREE1 * bt, void *op, void *key)
{
   return _bt1_search(bt, op, bt->root, key, NULL);
}
