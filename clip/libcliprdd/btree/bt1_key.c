void *
bt1_key(BTREE1 * bt)
{
   return KEY(bt->cur, bt->curpos);
}
