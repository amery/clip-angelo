static int
_ntx_leftkeys(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, unsigned int pageoffs, int *keyno, const char *__PROC__)
{
   NTX_PAGE page;

   NTX_BUCKET *buck;

   int i, er;

   if (pageoffs)
   {
      _ntx_page(ro, pageoffs, &page);
      *keyno += _rdd_ushort(page.nkeys);
      for (i = 0; i <= _rdd_ushort(page.nkeys); i++)
      {
	 buck = _ntx_buck(&page, i);
	 if (_rdd_uint(buck->left))
	    if ((er = _ntx_leftkeys(ClipMachineMemory, ro, _rdd_uint(buck->left), keyno, __PROC__)))
	       return er;
      }
   }
   return 0;
}
