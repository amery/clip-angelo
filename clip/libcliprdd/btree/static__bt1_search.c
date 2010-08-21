static int
_bt1_search(BTREE1 * bt, void *op, void *page, void *key, int *uniqfound)
{
   int i = 0;

   int c = 1;

   int l = 0;

   int h = NKEYS(page) - 1;

   if (LEFT(page, 0))
      h--;
   while (l <= h)
   {
      i = (l + h) / 2;
      c = bt->bt_compare(op, KEY(page, i), key, uniqfound);
      if (bt->unique && !*uniqfound)
	 return 0;
      if (c < 0)
	 l = i + 1;
      else if (c > 0)
	 h = i - 1;
      else
      {
	 break;
      }
   }
   if (!c || !LEFT(page, i))
   {
      bt->cur = page;
      bt->curpos = i;
      if (c < 0)
	 bt->curpos++;
   }
   else
   {
      if (c < 0)
	 i++;
      _bt1_search(bt, op, LEFT(page, i), key, uniqfound);
      if (bt->unique && !*uniqfound)
	 return 0;
   }
   return !c;
}
