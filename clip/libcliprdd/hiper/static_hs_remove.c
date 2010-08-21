static int
hs_remove(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int rec, const char *__PROC__)
{
   unsigned char c4[4];

   unsigned int poffs, offs;

   int i, er;

   for (i = sizeof(HS_HEADER); i < 0x40000 + sizeof(HS_HEADER); i += 4)
   {
      if ((er = rdd_read(ClipMachineMemory, &hs->file, i, 4, c4, __PROC__)))
	 goto err;
      poffs = _rdd_uint(c4);
      if (!poffs)
	 continue;
      if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
	 goto err;
      if (_rdd_uint(c4))
      {				/* binary */
	 int rs = (hs->intsonpage * (hs->intsonpage - 3)) << 5;

	 int prs = 0;

	 int f = 3;

	 unsigned int next, offs;

	 int pos;

	 while (rec > rs)
	 {
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (f << 2), 4, c4, __PROC__)))
	       goto err;
	    next = _rdd_uint(c4);
	    if (!next)
	    {
	       return 0;
	    }
	    poffs = next;
	    f = 0;
	    prs = rs;
	    rs += (hs->intsonpage * (hs->intsonpage - 1)) << 5;
	 }
	 pos = (rec - 1 - prs) / (hs->intsonpage << 5) + f;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
	    goto err;
	 offs = _rdd_uint(c4);
	 pos = (rec - 1 - prs - (pos - f) * (hs->intsonpage << 5)) >> 5;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
	    goto err;
	 _rdd_put_uint(c4, _rdd_uint(c4) & (~(1 << ((rec - 1) % 32))));
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
	    goto err;
      }
      else
      {
	 unsigned int npairs, roffs;

	 int l, h, i = 0, c, rpos, nextp;

	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
	    goto err;
	 npairs = _rdd_uint(c4);
	 l = 0;
	 h = npairs - 1;
	 while (l <= h)
	 {
	    i = (l + h) / 2;
	    if ((er = _hs_compare(ClipMachineMemory, hs, poffs, i, rec, &c, &offs, &roffs, &rpos, &nextp, __PROC__)))
	       goto err;
	    if (c < 0)
	       l = i + 1;
	    else if (c > 0)
	       h = i - 1;
	    else
	       break;
	 }
	 if (!c)
	 {
	    if ((er = _hs_delete(ClipMachineMemory, hs, offs, i % hs->intsonpage, roffs, rpos, nextp, __PROC__)))
	       goto err;
	    _rdd_put_uint(c4, npairs - 1);
	    if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
	       goto err;
	 }
      }
   }
   return 0;
 err:
   return er;
}
