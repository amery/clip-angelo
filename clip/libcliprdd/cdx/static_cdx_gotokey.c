static int
cdx_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__)
{
   CDX_PAGE page;

   int out, er;

   unsigned int c = 0;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   *ok = 0;
   if (ro->scopetop)
   {
      CDX_HEADER hdr;

      unsigned int topno;

      int found;

      _CDX_HEADER(ro, &hdr);
      ro->stack[0].page = _rdd_uint(hdr.root);
      if (!ro->descend)
      {
	 if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, NULL, __PROC__)))
	    return er;
      }
      if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, &topno, __PROC__)))
	 return er;
      keyno += topno - 1;
   }
   if (!ro->descend)
   {
      if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	 return er;
      if (out)
      {
	 rd->bof = rd->v_bof = rd->eof = 1;
	 return 0;
      }
      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, 12, &page, __PROC__)))
	 return er;
      while (c + _rdd_ushort(page.nkeys) < keyno)
      {
	 if (_rdd_uint(page.right) == 0xffffffff)
	 {
	    unsigned int lastrec;

	    if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	       return er;
	    rd->eof = 1;
	    if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	       return er;
	    return 0;
	 }
	 c += _rdd_ushort(page.nkeys);
	 ro->stack[ro->level].page = _rdd_uint(page.right);
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, _rdd_uint(page.right), 12, &page, __PROC__)))
	    return er;
      }
      ro->stack[ro->level].pos = keyno - c - 1;
      if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	 return er;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	 return er;
   }
   else
   {
      if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	 return er;
      if (out)
      {
	 rd->bof = rd->v_bof = rd->eof = 1;
	 return 0;
      }
      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, 12, &page, __PROC__)))
	 return er;
      while (c + _rdd_ushort(page.nkeys) < keyno)
      {
	 if (_rdd_uint(page.left) == 0xffffffff)
	 {
	    unsigned int lastrec;

	    if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	       return er;
	    rd->eof = 1;
	    if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	       return er;
	    return 0;
	 }
	 c += _rdd_ushort(page.nkeys);
	 ro->stack[ro->level].page = _rdd_uint(page.left);
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, _rdd_uint(page.left), 12, &page, __PROC__)))
	    return er;
      }
      ro->stack[ro->level].pos = _rdd_ushort(page.nkeys) - (keyno - c);
      if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	 return er;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	 return er;
   }
   if (ro->scopetop || ro->scopebottom)
   {
      ClipVar v, *vp;

      void *key = malloc(ro->bufsize);

      int ook;

      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	 return er;
      vp = _clip_vptr(&v);
      if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	 return er;
      _clip_destroy(ClipMachineMemory, &v);
      if ((ro->type == 'C' || (ro->type == 'X' && ((char *) key)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
	 loc_write(rd->loc, key, ro->bufsize);
      if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ook, __PROC__)))
	 return er;
      free(key);
      if (ook)
      {
	 unsigned int lastrec;

	 if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	    return er;
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
      else
      {
	 *ok = 1;
      }
   }
   else
   {
      *ok = 1;
   }
   return 0;
}
