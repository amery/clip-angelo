static int
_hs_loadbits(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, unsigned int lastrec, unsigned int **rmap, const char *__PROC__)
{
   unsigned char c4[4];

   int bitstyle;

   int rpos, pos;

   unsigned int *rpage = malloc(hs->pagesize);

   unsigned int *page = malloc(hs->pagesize);

   unsigned int next, offs;

   int ints = (lastrec + 31) / 32;

   int c = 0, er;

   *rmap = calloc(1, ints * 4);
   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
      goto err;
   bitstyle = _rdd_uint(c4);
   if (bitstyle)
   {
      int tail;

      rpos = 3;
      next = poffs;
      do
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, next, hs->pagesize, rpage, __PROC__)))
	    goto err;
	 for (; rpos < hs->intsonpage; rpos++)
	 {
	    offs = _rdd_uint((unsigned char *) (rpage + rpos));
	    if (!offs)
	       break;
	    tail = min(hs->pagesize, (ints - c) << 2);
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, tail, *rmap + c, __PROC__)))
	       goto err;
	    c += hs->intsonpage;
	 }
	 rpos = 1;
	 next = _rdd_uint((unsigned char *) (rpage + ((next == poffs) ? 2 : 0)));
      }
      while (next);
   }
   else
   {
      rpos = 3;
      next = poffs;
      do
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, next, hs->pagesize, rpage, __PROC__)))
	    goto err;
	 for (; rpos < hs->intsonpage; rpos++)
	 {
	    offs = _rdd_uint((unsigned char *) (rpage + rpos));
	    if (!offs)
	       break;
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
	       goto err;
	    for (pos = 0; pos < hs->intsonpage; pos++)
	    {
	       if (!page[pos])
		  break;
	       _rm_setbit(*rmap, lastrec, page[pos]);
	    }
	 }
	 rpos = 1;
	 next = _rdd_uint((unsigned char *) (rpage + ((next == poffs) ? 2 : 0)));
      }
      while (next);
   }
   free(rpage);
   free(page);
   return 0;
 err:
   free(*rmap);
   *rmap = NULL;
   free(rpage);
   free(page);
   return er;
}
