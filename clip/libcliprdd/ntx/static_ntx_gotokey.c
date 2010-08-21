static int
ntx_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_PAGE page;

   NTX_BUCKET *buck;

   int left, oneleft, i, lastkey, er;

   *ok = 1;
   if ((er = ntx_lastkey(ClipMachineMemory, rd, ro, &lastkey, __PROC__)))
      return er;
   if (!lastkey)
   {
      rd->bof = rd->v_bof = rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, 1, 0, __PROC__)))
	 return er;
      return 0;
   }

   _ntx_header(ro, &hdr);
   _ntx_page(ro, _rdd_uint(hdr.root), &page);
   left = 0;
   while (1)
   {
      for (i = 0; i <= _rdd_ushort(page.nkeys); i++)
      {
	 buck = _ntx_buck(&page, i);
	 oneleft = 0;
	 if ((er = _ntx_leftkeys(ClipMachineMemory, ro, _rdd_uint(buck->left), &oneleft, __PROC__)))
	    return er;
	 if (left + oneleft + 1 == keyno)
	 {
	    goto ok;
	 }
	 else if (left + oneleft + 1 > keyno)
	 {
	    _ntx_page(ro, _rdd_uint(buck->left), &page);
	    break;
	 }
	 left += oneleft + 1;
      }
   }
 ok:
   return rd->vtbl->rawgo(ClipMachineMemory, rd, _rdd_uint(buck->recno), 1, __PROC__);
}
