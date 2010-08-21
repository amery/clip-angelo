int
bt_last(BTREE * bt)
{
   return _bt_lastnode(bt, bt->root);
}
