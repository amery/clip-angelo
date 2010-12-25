static int
__ntx_last(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int root, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;

   NTX_BUCKET *buck;

   int er;

   if ((er = _ntx_loadpage(ClipMachineMemory, ro, root, __PROC__)))
      return er;
   buck = _ntx_buck(page, 0);
   ro->level++;

   if (ro->level >= MAX_BTREE_DEEP)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);

   ro->stack[ro->level].page = root;
   ro->stack[ro->level].pos = _rdd_ushort(page->nkeys);
   if (!_rdd_uint(buck->left))
   {
      if (ro->stack[ro->level].pos > 0)
      {
	 ro->stack[ro->level].pos--;
	 buck = _ntx_buck(page, ro->stack[ro->level].pos);
	 return rd->vtbl->rawgo(ClipMachineMemory, rd, _rdd_uint(buck->recno), 1, __PROC__);
      }
   }
   else
   {
      buck = _ntx_buck(page, ro->stack[ro->level].pos);
      if ((er = __ntx_last(ClipMachineMemory, rd, ro, _rdd_uint(buck->left), __PROC__)))
	 return er;
   }
   return 0;
}
