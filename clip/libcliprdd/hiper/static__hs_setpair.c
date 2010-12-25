static int
_hs_setpair(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, unsigned int rec, const char *__PROC__)
{
   unsigned char c4[4];

   void *buf = NULL;

   int binary, er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
      goto err;
   binary = _rdd_uint(c4);
   if (binary)
   {
      int rs = (hs->intsonpage * (hs->intsonpage - 3)) << 5;

      int prs = 0;

      int f = 3;

      unsigned int next, offs;

      int pos, i;

      buf = calloc(1, hs->pagesize);
      while (rec > rs)
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (f << 2), 4, c4, __PROC__)))
	    goto err;
	 next = _rdd_uint(c4);
	 if (!next)
	 {
	    if ((er = _hs_getfuu(ClipMachineMemory, hs, &next, __PROC__)))
	       goto err;
	    if ((er = rdd_write(ClipMachineMemory, &hs->file, next, hs->pagesize, buf, __PROC__)))
	       goto err;
	    _rdd_put_uint(c4, next);
	    if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + (f << 2), 4, c4, __PROC__)))
	       goto err;

	    pos = hs->intsonpage - 1;
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
	       goto err;
	    while (pos > f && _rdd_uint(c4) == 0)
	    {
	       if ((er = _hs_getfuu(ClipMachineMemory, hs, &offs, __PROC__)))
		  goto err;
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
		  goto err;
	       _rdd_put_uint(c4, offs);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
		  goto err;
	    }
	 }
	 poffs = next;
	 f = 0;
	 prs = rs;
	 rs += (hs->intsonpage * (hs->intsonpage - 1)) << 5;
      }
      i = pos = (rec - 1 - prs) / (hs->intsonpage << 5) + f;
      if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (i << 2), 4, c4, __PROC__)))
	 goto err;
      while (i > f && _rdd_uint(c4) == 0)
      {
	 if ((er = _hs_getfuu(ClipMachineMemory, hs, &offs, __PROC__)))
	    goto err;
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
	    goto err;
	 _rdd_put_uint(c4, offs);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + (i << 2), 4, c4, __PROC__)))
	    goto err;
      }
      if (buf)
      {
	 free(buf);
	 buf = NULL;
      }
      if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
	 goto err;
      offs = _rdd_uint(c4);
      pos = (rec - 1 - prs - (pos - f) * (hs->intsonpage << 5)) >> 5;
      if ((er = rdd_read(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
	 goto err;
      _rdd_put_uint(c4, _rdd_uint(c4) | (1 << ((rec - 1) % 32)));
      if ((er = rdd_write(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
	 goto err;
   }
   else
   {
      unsigned int npairs, noffs = 0, offs, roffs, npairs1, npairsN;

      int tobit, l, h, i = 0, c, rpos, nextp;

      if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
	 goto err;
      npairs = _rdd_uint(c4);
      if ((npairs % hs->intsonpage) == 0)
      {
	 if ((er = _hs_tobitstyle(ClipMachineMemory, hs, poffs, rec, npairs, &tobit, __PROC__)))
	    goto err;
	 if (tobit)
	    return _hs_setpair(ClipMachineMemory, hs, poffs, rec, __PROC__);
	 buf = calloc(1, hs->pagesize);
	 npairs1 = hs->intsonpage * (hs->intsonpage - 3);
	 npairsN = hs->intsonpage * (hs->intsonpage - 1);
	 if ((npairs == npairs1) || ((npairs > npairs1) && ((npairs - npairs1) % npairsN == 0)))
	 {
	    if (_hs_getfuu(ClipMachineMemory, hs, &noffs, __PROC__))
	       goto err;
	    if ((er = rdd_write(ClipMachineMemory, &hs->file, noffs, hs->pagesize, buf, __PROC__)))
	       goto err;
	    offs = poffs + 8;
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	       goto err;
	    while (_rdd_uint(c4))
	    {
	       offs = _rdd_uint(c4);
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
		  goto err;
	    }
	    _rdd_put_uint(c4, noffs);
	    if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	       goto err;
	 }
	 if ((er = _hs_getfuu(ClipMachineMemory, hs, &roffs, __PROC__)))
	    goto err;
	 offs = poffs + 8;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	    goto err;
	 while (_rdd_uint(c4))
	 {
	    offs = _rdd_uint(c4);
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	       goto err;
	 }
	 offs += 4;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	    goto err;
	 while (_rdd_uint(c4))
	 {
	    offs += 4;
	    if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	       goto err;
	 }
	 _rdd_put_uint(c4, roffs);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	    goto err;
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, roffs, hs->pagesize, buf, __PROC__)))
	    goto err;
	 free(buf);
	 buf = NULL;
      }
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
	    return 0;
      }
      if ((er = _hs_insert(ClipMachineMemory, hs, offs, i % hs->intsonpage + 1, rec, noffs ? noffs : roffs, noffs ? 0 : rpos, nextp, __PROC__)))
	 goto err;
      _rdd_put_uint(c4, npairs + 1);
      if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
	 goto err;
   }
   return 0;
 err:
   if (buf)
      free(buf);
   return er;
}
