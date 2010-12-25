static int
_ntx_last(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int root, unsigned int *recno, int *out, const char *__PROC__)
{
   NTX_PAGE page;

   NTX_BUCKET *buck;

   int er;

   _ntx_page(ro, root, &page);
   buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
   ro->stack[ro->level].page = root;
   ro->stack[ro->level].pos = _rdd_ushort(page.nkeys);
   if (out)
   {
      *out = 0;
      if (_rdd_ushort(page.nkeys) == 0)
      {
	 *out = 1;
	 return 0;
      }
   }
   while (_rdd_uint(buck->left))
   {
      ro->level++;
      ro->stack[ro->level].page = _rdd_uint(buck->left);
      _ntx_page(ro, _rdd_uint(buck->left), &page);
      ro->stack[ro->level].pos = _rdd_ushort(page.nkeys);
      buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
   }
   ro->stack[ro->level].pos = _rdd_ushort(page.nkeys) - 1;
   buck = _ntx_buck(&page, _rdd_ushort(page.nkeys) - 1);
   *recno = _rdd_uint(buck->recno);
   return 0;
}
