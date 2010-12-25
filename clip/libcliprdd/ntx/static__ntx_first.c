static int
_ntx_first(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int root, unsigned int *recno, int *out, const char *__PROC__)
{
   NTX_PAGE page;

   NTX_BUCKET *buck;

   int er;

   _ntx_page(ro, root, &page);
   buck = _ntx_buck(&page, 0);
   ro->stack[ro->level].page = root;
   ro->stack[ro->level].pos = 0;
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
      ro->stack[ro->level].pos = 0;
      _ntx_page(ro, _rdd_uint(buck->left), &page);
      buck = _ntx_buck(&page, 0);
   }
   *recno = _rdd_uint(buck->recno);
   return 0;
}
