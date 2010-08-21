static int
_bt1_last(BTREE1 * bt, void *root)
{
   bt->cur = root;
   while (LEFT(bt->cur, NKEYS(bt->cur) - 1))
      bt->cur = LEFT(bt->cur, NKEYS(bt->cur) - 1);
   bt->curpos = NKEYS(bt->cur) - 1;
   return 0;
}
