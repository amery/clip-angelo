int
_rdd_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__)
{
   int er;

   if (fp && fp->optimize == 2 && ClipMachineMemory->optimizelevel > 0)
   {
      if (rd->curord == -1)
      {
	 if ((er = rd->vtbl->calcfiltlist(ClipMachineMemory, rd, fp, __PROC__)))
	    return er;
      }
      else
      {
	 RDD_ORDER *ro = rd->orders[rd->curord];

	 if (ro->vtbl->calcfiltlist)
	 {
	    if ((er = ro->vtbl->calcfiltlist(ClipMachineMemory, rd, ro, fp, __PROC__)))
	       return er;
	 }
      }
      if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
	 return er;
   }
   return 0;
}
