static int
_hs_tobitstyle(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, unsigned int rec, unsigned int npairs, int *tobit, const char *__PROC__)
{
   int bints = (max(hs->lastrec, rec) + 31) / 32;

   unsigned int *rmap = NULL;

   int er;

   *tobit = 0;
   if (bints < npairs + 1)
   {
      unsigned char c4[4];

      int c = 0;

      int next = 8;

      int pos = 12;

      int tail;

      if ((er = _hs_loadbits(ClipMachineMemory, hs, poffs, max(hs->lastrec, rec), &rmap, __PROC__)))
	 goto err;

      *tobit = 1;
      _rdd_put_uint(c4, 1);
      if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
	 goto err;

      while (c < bints)
      {
	 if (pos == hs->intsonpage)
	 {
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + next, 4, c4, __PROC__)))
	       goto err;
	    poffs = _rdd_uint(c4);
	    next = 0;
	    pos = 4;
	 }
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + pos, 4, c4, __PROC__)))
	    goto err;
	 tail = min(hs->pagesize, (bints - c) * 4);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, _rdd_uint(c4), tail, rmap + c, __PROC__)))
	    goto err;
	 if (tail < hs->pagesize)
	 {
	    void *buf = calloc(1, hs->pagesize - tail);

	    if ((er = rdd_write(ClipMachineMemory, &hs->file, _rdd_uint(c4) + tail, hs->pagesize - tail, buf, __PROC__)))
	    {
	       free(buf);
	       goto err;
	    }
	    free(buf);
	 }
	 c += hs->intsonpage;
	 pos += 4;
      }
      free(rmap);
   }
   return 0;
 err:
   if (rmap)
      free(rmap);
   return er;
}
