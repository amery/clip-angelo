static int
_cdx_last(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, int *out, const char *__PROC__)
{
   CDX_HEADER hdr;

   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   int er;

   ro->level = 0;
   *out = 0;
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if (!(_rdd_ushort(page.attrs) & 0x1))	/* not root page */
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badroot);

   if (_rdd_ushort(page.nkeys) == 0)
   {
      *out = 1;
      return 0;
   }

   ro->stack[0].pos = _rdd_ushort(page.nkeys) - 1;
   while (!leaf)
   {
      (ro->level)++;
      if ((ro->level) >= MAX_BTREE_DEEP)
	 return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
      if ((er = _cdx_recno(ClipMachineMemory, ro, (ro->level) - 1, &ro->stack[ro->level].page, __PROC__)))
	 return er;
      _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
      ro->stack[ro->level].pos = _rdd_ushort(page.nkeys) - 1;
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
