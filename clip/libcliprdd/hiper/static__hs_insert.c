static int
_hs_insert(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int offs, int pos, unsigned int rec, unsigned int roffs, int rpos, int nextp, const char *__PROC__)
{
   unsigned char c4[4];

   unsigned int *page = malloc(hs->pagesize + 4);

   int i, er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
      goto err;
   memmove(page + pos + 1, page + pos, hs->pagesize - pos * 4);
   _rdd_put_uint((unsigned char *) (page + pos), rec);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
      goto err;
   do
   {
      for (i = rpos + 1; i < hs->intsonpage; i++)
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + i * 4, 4, c4, __PROC__)))
	    goto err;
	 offs = _rdd_uint(c4);
	 if (!offs)
	    break;

	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
	    goto err;
	 rec = _rdd_uint((unsigned char *) (page + hs->intsonpage));
	 memmove(page + 1, page, hs->pagesize);
	 _rdd_put_uint((unsigned char *) page, rec);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
	    goto err;
      }
      if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + nextp, 4, c4, __PROC__)))
	 goto err;
      roffs = _rdd_uint(c4);
   }
   while (roffs);

   free(page);
   return 0;
 err:
   free(page);
   return er;
}
