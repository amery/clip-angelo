static int
_ntx_savebtree(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, BTREE * bt, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_PAGE page;

   int eof;

   char *key;

   unsigned int poffs;

   unsigned int oldrecno = rd->recno;

   int f = 1, er;

   _ntx_header(ro, &hdr);
   poffs = _rdd_uint(hdr.root);
   _ntx_page(ro, poffs, &page);
   eof = bt_first(bt);
   while (!eof)
   {
      if (_rdd_ushort(page.nkeys) == ro->fullpage)
      {
	 NTX_BUCKET *buck;

	 if ((er = rdd_write(ClipMachineMemory, &ro->index->file, poffs, sizeof(NTX_PAGE), &page, __PROC__)))
	    return er;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, *((unsigned int *) bt_key(bt)), 0, __PROC__)))
	    return er;
	 rd->eof = 0;
	 if ((er = ntx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	    return er;
	 if (bt_next(bt))
	 {
	    f = 0;
	    break;
	 }
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->header + 4, sizeof(unsigned int), &poffs, __PROC__)))
	    return er;
	 _ntx_page(ro, poffs, &page);
	 buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
	 while (_rdd_uint(buck->left))
	 {
	    poffs = _rdd_uint(buck->left);
	    _ntx_page(ro, _rdd_uint(buck->left), &page);
	    buck = _ntx_buck(&page, _rdd_ushort(page.nkeys));
	 }
	 _ntx_page(ro, poffs, &page);
      }
      key = (char *) (&page) + _rdd_ushort(&page.offs[_rdd_ushort(page.nkeys) * sizeof(unsigned short)]) + 4;
      memcpy(key, (char *) bt_key(bt), ro->keysize + 4);
      if (ro->type == 'C')
	 loc_write(ro->index->loc, (unsigned char *) (key + sizeof(unsigned int)), ro->keysize);
      eof = bt_next(bt);
      _rdd_put_ushort(page.nkeys, _rdd_ushort(page.nkeys) + 1);
   }
   if (f)
   {
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, poffs, sizeof(NTX_PAGE), &page, __PROC__)))
	 return er;
   }
   return rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__);
}
