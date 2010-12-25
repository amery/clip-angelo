static int
_bt1_first(BTREE1 * bt, void *root)
{
   bt->cur = root;
   while (LEFT(bt->cur, 0))
      bt->cur = LEFT(bt->cur, 0);
   bt->curpos = 0;
   return 0;
}
