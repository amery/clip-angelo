static int
_cdx_search_tree(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, void *key, int len, unsigned int recno, int *found, int *out, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er;

   *found = 0;
   if (out)
      *out = 0;
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   while (!leaf)
   {
      int i;

      for (i = 0; i < _rdd_ushort(branch->nkeys); i++)
      {
	 unsigned int recno1 = _rdd_backuint(branch->keys + i * (ro->bufsize + 8) + ro->bufsize);

	 int r;

	 r = _cdx_cmp(ro, branch->keys + i * (ro->bufsize + 8), key, len);
	 if ((r > 0) || ((r == 0) && (recno1 >= recno)) || (i == _rdd_ushort(branch->nkeys) - 1))
	 {

	    ro->stack[ro->level].pos = i;
	    (ro->level)++;
	    if ((ro->level) >= MAX_BTREE_DEEP)
	       return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
	    if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
	       return er;
	    _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
	    break;
	 }
      }
   }
   *found = _cdx_search_leaf(ro, leaf, key, len, recno, &(ro->stack[ro->level].pos), 0);
   if (ro->stack[ro->level].pos < 0)
      if (out)
	 *out = -1;
   if (ro->stack[ro->level].pos == _rdd_ushort(leaf->nkeys))
      if (out)
	 *out = 1;
   return 0;
}
