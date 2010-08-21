static int
idx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__)
{
   int er;

   if ((er = _cdx_create(ClipMachineMemory, rd, ri, rop, ri->name, expr, block, unique, 0, __PROC__)))
      goto err;

   return 0;
 err:
   return er;
}
