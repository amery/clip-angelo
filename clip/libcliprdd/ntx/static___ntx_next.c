static int
__ntx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *out, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;

   NTX_BUCKET *buck;

   int level, er;

   *out = 0;
   level = ro->level;

   if ((er = _ntx_loadpage(ClipMachineMemory, ro, ro->stack[level].page, __PROC__)))
      return er;
   buck = _ntx_buck(page, ro->stack[level].pos);

   if (!_rdd_uint(buck->left))
   {
      if (ro->stack[level].pos < _rdd_ushort(page->nkeys) - 1)
      {
	 ro->stack[level].pos++;
	 buck = _ntx_buck(page, ro->stack[level].pos);
	 return rd->vtbl->rawgo(ClipMachineMemory, rd, _rdd_uint(buck->recno), 1, __PROC__);
      }
      else
      {
	 while (--level >= 0)
	 {
	    if ((er = _ntx_loadpage(ClipMachineMemory, ro, ro->stack[level].page, __PROC__)))
	       return er;
	    if (ro->stack[level].pos < _rdd_ushort(page->nkeys))
	       break;
	 }
	 if (level < 0)
	 {
	    *out = 1;
	 }
	 else
	 {
	    ro->level = level;
	    buck = _ntx_buck(page, ro->stack[level].pos);
	    return rd->vtbl->rawgo(ClipMachineMemory, rd, _rdd_uint(buck->recno), 1, __PROC__);
	 }
      }
   }
   else
   {
      ro->stack[level].pos++;
      buck = _ntx_buck(page, ro->stack[level].pos);
      if ((er = __ntx_first(ClipMachineMemory, rd, ro, _rdd_uint(buck->left), __PROC__)))
	 return er;
   }
   return 0;
}
