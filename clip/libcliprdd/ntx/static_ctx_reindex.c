static int
ctx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro;

   int i, er;

   if ((er = ctx_zap(ClipMachineMemory, rd, ri, __PROC__)))
      return er;
   for (i = 0; i < ri->norders; i++)
   {
      ro = ri->orders[i];
      if ((er = _ntx_reindex(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;
   }
   return 0;
}
