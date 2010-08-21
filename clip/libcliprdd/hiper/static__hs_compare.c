static int
_hs_compare(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, int pos, unsigned int rec, int *c, unsigned int *offs, unsigned int *roffs, int *rpos, int *nextp, const char *__PROC__)
{
   int curr = hs->intsonpage * (hs->intsonpage - 3);

   int prev = 0;

   unsigned char c4[4];

   int lpos, er;

   *nextp = 8;
   while (pos >= curr)
   {
      if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (prev ? 0 : 8), 4, c4, __PROC__)))
	 goto err;
      poffs = _rdd_uint(c4);
      prev = curr;
      curr += hs->intsonpage * (hs->intsonpage - 1);
      *nextp = 0;
   }
   *roffs = poffs;
   lpos = (pos - prev) / hs->intsonpage;
   *rpos = lpos + (prev ? 1 : 3);
   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (prev ? 0 : 8) + 4 + lpos * 4, 4, c4, __PROC__)))
      goto err;
   poffs = _rdd_uint(c4);
   lpos = pos % hs->intsonpage;
   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + lpos * 4, 4, c4, __PROC__)))
      goto err;
   lpos = _rdd_uint(c4);
   if (lpos < rec)
      *c = -1;
   else if (lpos > rec)
      *c = 1;
   else
      *c = 0;
   *offs = poffs;
   return 0;
 err:
   return er;
}
