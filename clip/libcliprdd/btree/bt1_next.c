int
bt1_next(BTREE1 * bt)
{
   if (!LEFT(bt->cur, 0))
   {
      bt->curpos++;
      if (NKEYS(bt->cur) <= bt->curpos)
      {
	 bt->curpos = PARPOS(bt->cur);
	 bt->cur = PARENT(bt->cur);
	 while (bt->cur && NKEYS(bt->cur) - 1 == bt->curpos)
	 {
	    bt->curpos = PARPOS(bt->cur);
	    bt->cur = PARENT(bt->cur);
	 }
	 if (!bt->cur)
	    return 1;
      }
   }
   else
   {
      bt->curpos++;
      _bt1_first(bt, LEFT(bt->cur, bt->curpos));
   }
   return 0;
}
