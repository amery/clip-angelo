static int
__ntx_first(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int root, const char *__PROC__)
{
   NTX_PAGE *page = ro->curpage;

   NTX_BUCKET *buck;

   int er;

   do
   {
      ro->level++;
      if (ro->level >= MAX_BTREE_DEEP)
	 return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
      ro->stack[ro->level].page = root;
      ro->stack[ro->level].pos = 0;
      if ((er = _ntx_loadpage(ClipMachineMemory, ro, root, __PROC__)))
	 return er;
      buck = _ntx_buck(page, 0);
      root = _rdd_uint(buck->left);
   }
   while (root);
   return rd->vtbl->rawgo(ClipMachineMemory, rd, _rdd_uint(buck->recno), 1, __PROC__);
}
