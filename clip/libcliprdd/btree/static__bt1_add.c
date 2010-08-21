static int
_bt1_add(BTREE1 * bt, void *page, short pos, void *key, void *left, void **lpar, short *lpos, void **rpar, short *rpos)
{
   short nkeys = NKEYS(page);

   short ipos = POS(page, nkeys);

   short i;

   memmove(&POS(page, pos + 1), &POS(page, pos), sizeof(short) * (nkeys - pos));
   POS(page, pos) = ipos;
   memcpy(KEY(page, pos), key, bt->keysize);
   LEFT(page, pos) = left;
   (NKEYS(page))++;
   if (lpar)
   {
      *lpar = *rpar = page;
      *lpos = *rpos = pos;	//PARPOS(page);
      (*rpos)++;
   }
   if (LEFT(page, 0))
   {
      for (i = pos + 1; i <= nkeys; i++)
      {
	 PARPOS(LEFT(page, i)) = i;
      }
   }
   if (nkeys == bt->keysonpage - 1)
   {
      void *lp = _bt_newpage(bt);

      void *pp = PARENT(page);

      void *rrpar, *llpar;

      short rrpos, llpos;

      int i, j;

      for (i = 0, j = bt->halfpage + 1; i <= bt->halfpage; i++, j++)
      {
	 memcpy(KEY(lp, i), KEY(page, i), bt->recsize);
	 if (j <= nkeys)
	    memcpy(KEY(page, i), KEY(page, j), bt->recsize);
	 if (LEFT(lp, i))
	 {
	    PARENT(LEFT(lp, i)) = lp;
	    PARPOS(LEFT(lp, i)) = i;
	    if (j <= nkeys)
	       PARPOS(LEFT(page, i)) = i;
	 }
      }
      if (!pp)
      {
	 bt->root = pp = _bt_newpage(bt);
	 LEFT(pp, 0) = page;
	 NKEYS(pp) = 1;
      }
      if (pos /*PARPOS(page) */  < bt->halfpage)
      {
	 if (lpar)
	 {
	    *lpar = *rpar = lp;
	    *lpos = *rpos = pos;	//PARPOS(page);
	    (*rpos)++;
	 }
      }
      else if (pos /*PARPOS(page) */  > bt->halfpage)
      {
	 if (lpar)
	 {
	    *lpar = *rpar = page;
	    *lpos = *rpos = pos /*PARPOS(page) */  - bt->halfpage - 1;
	    (*rpos)++;
	 }
      }
      else
      {
	 if (lpar)
	 {
	    *lpar = lp;
	    *rpar = page;
	    *lpos = pos;	//PARPOS(page);
	    *rpos = 0;
	 }
      }
      NKEYS(lp) = bt->halfpage + (LEFT(lp, 0) != 0);
      NKEYS(page) = nkeys - bt->halfpage;
      if (_bt1_add(bt, pp, PARPOS(page), KEY(page, bt->halfpage), lp, &llpar, &llpos, &rrpar, &rrpos))
	 return 1;
      PARENT(lp) = llpar;
      PARPOS(lp) = llpos;
      PARENT(page) = rrpar;
      PARPOS(page) = rrpos;
   }
   return 0;
}
