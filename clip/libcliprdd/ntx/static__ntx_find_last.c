static int
_ntx_find_last(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_PAGE pp;

   NTX_BUCKET *bp;

   unsigned int right;

   int count, er;

   unsigned int page = ro->stack[ro->level].page;

   _ntx_page(ro, page, &pp);
   count = _rdd_ushort(pp.nkeys);
   bp = _ntx_buck(&pp, count);
   right = _rdd_uint(bp->left);

   if (right)
   {
      ro->stack[ro->level].pos = count;
      (ro->level)++;
      if (ro->level >= MAX_BTREE_DEEP)
	 return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
      ro->stack[ro->level].page = right;
      return _ntx_find_last(ClipMachineMemory, ro, __PROC__);
   }
   ro->stack[ro->level].pos = count - 1;
   return 0;
}
