static int
ntx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__)
{
   RDD_ORDER *ro = ri->orders[0];

   int er;

   if ((er = ntx_zap(ClipMachineMemory, rd, ri, __PROC__)))
      return er;
   if ((er = _ntx_reindex(ClipMachineMemory, rd, ro, __PROC__)))
      return er;
   return 0;
}
