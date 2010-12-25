static int
cdx_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__)
{
   CDX_HEADER hdr;

   void *key = malloc(ro->bufsize);

   int ok, fok, len, outrange, er;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = cdx_formatkey(ClipMachineMemory, ro, v, key, __PROC__)))
      return er;

   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      len = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize - (ro->type == 'X' ? 2 : 0));
   else
      len = ro->bufsize;

   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((last && !ro->descend) || (!last && ro->descend))
   {
      if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, len, 0, found, &outrange, __PROC__)))
	 return er;
      if (last && outrange < 0)
      {
	 outrange = 0;
	 ro->stack[ro->level].pos = 0;
      }
      else if (!last && outrange > 0)
      {
	 outrange = 0;
	 ro->stack[ro->level].pos--;
      }
   }
   else
   {
      if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, len, 0, found, &outrange, __PROC__)))
	 return er;
   }

   if (outrange)
   {
      unsigned int lastrec;

      if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	 return er;
      rd->eof = 1;
      free(key);
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      if (rd->area != -1)
	 ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 0;
      return 0;
   }
   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
      return er;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
      return er;
   if ((ro->type == 'C' || (ro->type == 'X' && ((char *) key)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_write(rd->loc, key, ro->bufsize);
   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
      return er;
   if (!ok)
   {
      if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	 return er;
      if (!fok)
      {
	 ClipVar v, *vp;

	 void *k = malloc(ro->bufsize);

	 if ((er = cdx_next(ClipMachineMemory, rd, ro, __PROC__)))
	    return er;
	 if (ro->custom)
	 {
	    memcpy(k, ro->key, ro->keysize);
	 }
	 else
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	       return er;
	    vp = _clip_vptr(&v);
	    if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, k, __PROC__)))
	       return er;
	    _clip_destroy(ClipMachineMemory, &v);
	 }
	 *found = !_cdx_cmp(ro, k, key, len);
	 free(k);
      }
   }
   free(key);
   if (!(*found) && (!soft || ok))
   {
      unsigned int lastrec;

      if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	 return er;
      rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      ro->valid_stack = 0;
      if (rd->area != -1)
	 ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 0;
   }
   else
   {
      if (rd->area != -1)
	 ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 1;
      ro->valid_stack = 1;
   }

   return 0;
}
