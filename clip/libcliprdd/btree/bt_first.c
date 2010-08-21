int
bt_first(BTREE * bt)
{
   return _bt_firstnode(bt, bt->root);
}
