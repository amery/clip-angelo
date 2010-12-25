static int
_hs_delete(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int offs, int pos, unsigned int roffs, int rpos, int nextp, const char *__PROC__)
{
   unsigned int *buf = calloc(1, hs->pagesize);

   unsigned char c4[4];

   unsigned int next;

   unsigned int lp;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
      goto err;
   memmove(buf + pos, buf + pos + 1, hs->pagesize - 4 - (pos << 2));
   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
      goto err;
   lp = offs + hs->pagesize - 4;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + nextp, 4, c4, __PROC__)))
      goto err;
   next = _rdd_uint(c4);
   while (next)
   {
      for (; rpos < hs->intsonpage; rpos++)
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + (rpos << 2), 4, c4, __PROC__)))
	    goto err;
	 offs = _rdd_uint(c4);
	 if (!offs)
	    break;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
	    goto err;
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, lp, 4, buf, __PROC__)))
	    goto err;
	 memmove(buf, buf + 1, hs->pagesize - 4);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
	    goto err;
	 lp = offs + hs->pagesize - 4;
      }
      roffs = next;

      rpos = 1;
      if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs, 4, c4, __PROC__)))
	 goto err;
      next = _rdd_uint(c4);
   }

   free(buf);
   return 0;
 err:
   free(buf);
   return er;
}
