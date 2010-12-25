static int
_cdx_next_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * leaf, int *out, int cache, const char *__PROC__)
{
   int er;

   *out = 0;
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   ro->stack[ro->level].pos++;
   if (ro->stack[ro->level].pos >= _rdd_ushort(leaf->nkeys))
   {
      ro->stack[ro->level].pos = _rdd_ushort(leaf->nkeys);
      if (_rdd_uint(leaf->right) != 0xffffffff)
      {
	 ro->stack[ro->level].page = _rdd_uint(leaf->right);
	 if (cache)
	 {
	    if ((er = _cdx_loadpage(ClipMachineMemory, ro, ro->stack[ro->level].page, __PROC__)))
	       return er;
	 }
	 else
	 {
	    if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_PAGE), leaf, __PROC__)))
	       return er;
	 }

	 if (_rdd_ushort(leaf->nkeys) == 0)
	    return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_nokeys);
	 ro->stack[ro->level].pos = 0;
      }
      else
      {
	 *out = 1;
      }
   }
#ifndef D20070109
   if (*out != 1)
   {
      int recno;

      _cdx_leaf_item(ro, leaf, ro->stack[ro->level].pos, &recno, ro->key);
   }
#endif
   return 0;
}
