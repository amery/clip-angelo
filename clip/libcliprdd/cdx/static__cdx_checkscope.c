static int
_cdx_checkscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, void *key, int *ok, const char *__PROC__)
{
   int lok = 1, hok = 1;

   *ok = 0;
   if (!ro->scopetop && !ro->scopebottom)
      return 0;

   if (ro->scopetop)
   {
      int r;

      r = _cdx_cmp(ro, key, (unsigned char *) ro->scopetop, ro->stoplen);
      lok = (ro->descend ? (r <= 0) : (r >= 0));
   }
   if (ro->scopebottom)
   {
      int r;

      r = _cdx_cmp(ro, key, (unsigned char *) ro->scopebottom, ro->sbotlen);
      hok = (ro->descend ? (r >= 0) : (r <= 0));
   }

   if (lok && hok)
      *ok = 0;
   else if (!lok)
      *ok = -1;
   else
      *ok = 1;

   return 0;
}
