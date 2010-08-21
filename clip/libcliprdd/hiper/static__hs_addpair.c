static int
_hs_addpair(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int offs, unsigned int rec, const char *__PROC__)
{
   unsigned int roffs, poffs;

   char *page = calloc(1, hs->pagesize);

   unsigned char c4[4];

   int er;

   if ((er = _hs_getfuu(ClipMachineMemory, hs, &roffs, __PROC__)))
      goto err;
   if ((er = _hs_getfuu(ClipMachineMemory, hs, &poffs, __PROC__)))
      goto err;

   if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs, hs->pagesize, page, __PROC__)))
      goto err;

   _rdd_put_uint((unsigned char *) (page + 4), 1);
   _rdd_put_uint((unsigned char *) (page + 12), poffs);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, roffs, hs->pagesize, page, __PROC__)))
      goto err;

   _rdd_put_uint(c4, roffs);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
      goto err;

   _rdd_put_uint(c4, rec);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
      goto err;

   free(page);
   return 0;
 err:
   free(page);
   return er;
}
