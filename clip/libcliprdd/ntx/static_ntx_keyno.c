static int
ntx_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_PAGE page;

   NTX_BUCKET *buck;

   int r, i, j, er;

   *keyno = 0;
   if (rd->eof)
      return 0;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
      return er;

   if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;
   if (r)
      return 0;

   _ntx_page(ro, ro->stack[ro->level].page, &page);

   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   if (rd->recno != _rdd_uint(buck->recno))
      return 0;

   *keyno += ro->stack[ro->level].pos + 1;
   for (i = 0; i <= ro->stack[ro->level].pos; i++)
   {
      buck = _ntx_buck(&page, i);
      if ((er = _ntx_leftkeys(ClipMachineMemory, ro, _rdd_uint(buck->left), keyno, __PROC__)))
	 return er;
   }
   for (j = 0; j < ro->level; j++)
   {
      _ntx_page(ro, ro->stack[j].page, &page);
      *keyno += ro->stack[j].pos;
      for (i = 0; i < ro->stack[j].pos; i++)
      {
	 buck = _ntx_buck(&page, i);
	 if ((er = _ntx_leftkeys(ClipMachineMemory, ro, _rdd_uint(buck->left), keyno, __PROC__)))
	    return er;
      }
   }
   return 0;
}
