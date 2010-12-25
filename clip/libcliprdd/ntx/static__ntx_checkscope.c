static int
_ntx_checkscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *res, const char *__PROC__)
{
   int er;

   *res = 1;
   if (ro->scopetop || ro->scopebottom)
      if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;
   if (ro->scopetop)
      if (memcmp(ro->scopetop, ro->key, ro->stoplen) > 0)
	 *res = 0;
   if (ro->scopebottom)
      if (memcmp(ro->scopebottom, ro->key, ro->sbotlen) < 0)
	 *res = 0;
   return 0;
}
