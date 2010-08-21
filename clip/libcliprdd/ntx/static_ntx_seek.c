static int
ntx_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_BUCKET *buck;

   NTX_PAGE page;

   char expr[1024];

   int res, sok, lastrec, oldrecno, er;

   int len = ro->keysize;

   rd->bof = rd->v_bof = rd->eof = 0;
   if ((er = ntx_formatkey(ClipMachineMemory, ro, v, expr, __PROC__)))
      return er;
   if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      len = v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, expr, len, 0, &res, 0, __PROC__)))
      return er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   while (!_rdd_uint(buck->recno) && (ro->level > 0))
   {
      ro->level--;
      _ntx_page(ro, ro->stack[ro->level].page, &page);
      buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   }
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, _rdd_uint(buck->recno), 1, __PROC__)))
      return er;
   *found = !res;

   if (*found)
   {
      if (!ro->unique)
      {
	 if (last)
	 {
	    while (1)
	    {
	       RDD_STACK stack[MAX_BTREE_DEEP];

	       int level = ro->level;

	       int i;

	       for (i = 0; i <= level; i++)
		  stack[i] = ro->stack[i];
	       oldrecno = rd->recno;
	       if ((er = ntx_next(ClipMachineMemory, rd, ro, __PROC__)))
		  return er;
	       if (rd->eof)
	       {
		  if (rd->bof)
		  {
		     *found = 0;
		     break;
		  }
		  rd->eof = 0;
		  if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__)))
		     return er;
		  for (i = 0; i <= level; i++)
		     ro->stack[i] = stack[i];
		  ro->level = level;
		  if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
		     return er;
		  if ((er = _ntx_checkscope(ClipMachineMemory, rd, ro, &sok, __PROC__)))
		     return er;
		  if (!res || !sok)
		  {
		     if ((er = ntx_prev(ClipMachineMemory, rd, ro, __PROC__)))
			return er;
		     if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
			return er;
		     loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
		     *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
		  }
		  break;
	       }
	       if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
		  return er;
	       loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
	       if (_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0))
	       {
		  if ((er = ntx_prev(ClipMachineMemory, rd, ro, __PROC__)))
		     return er;
		  if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
		     return er;
		  loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
		  *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
		  break;
	       }
	    }
	 }
	 else
	 {
	    while (1)
	    {
	       RDD_STACK stack[MAX_BTREE_DEEP];

	       int level = ro->level;

	       int i;

	       for (i = 0; i <= level; i++)
		  stack[i] = ro->stack[i];
	       oldrecno = rd->recno;
	       if ((er = ntx_prev(ClipMachineMemory, rd, ro, __PROC__)))
		  return er;
	       if (rd->bof)
	       {
		  if (rd->eof)
		  {
		     *found = 0;
		     break;
		  }
		  rd->bof = rd->v_bof = 0;
		  if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__)))
		     return er;
		  for (i = 0; i <= level; i++)
		     ro->stack[i] = stack[i];
		  ro->level = level;
		  if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
		     return er;
		  if ((er = _ntx_checkscope(ClipMachineMemory, rd, ro, &sok, __PROC__)))
		     return er;
		  if (!res || !sok)
		  {
		     if ((er = ntx_next(ClipMachineMemory, rd, ro, __PROC__)))
			return er;
		     if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
			return er;
		     loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
		     *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
		  }
		  break;
	       }
	       if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
		  return er;
	       loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
	       if (_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0))
	       {
		  if ((er = ntx_next(ClipMachineMemory, rd, ro, __PROC__)))
		     return er;
		  if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
		     return er;
		  loc_write(rd->loc, (unsigned char *) ro->key, ro->keysize);
		  *found = !_ntx_compare(rd, 0, ro->descend, ro->key, ro->keysize, 0, expr, len, 0);
		  break;
	       }
	    }
	 }
      }
      else
      {
	 if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
	    return er;
	 if ((er = _ntx_checkscope(ClipMachineMemory, rd, ro, &sok, __PROC__)))
	    return er;
	 if (!res || !sok)
	 {
	    if ((er = ntx_next(ClipMachineMemory, rd, ro, __PROC__)))
	       return er;
	    *found = 0;
	 }
      }
   }

   if (*found)
   {
      ro->valid_stack = 1;
      if (rd->area != -1)
	 ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 1;
      return 0;
   }
   else if (rd->area != -1)
   {
      ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[rd->area]))->found = 0;
   }

   if (!soft || (rd->recno == 0))
   {
      if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	 return er;
      rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
   }
   else
   {
      if ((er = rdd_checkfilter(ClipMachineMemory, rd, &res, __PROC__)))
	 return er;
      if ((er = _ntx_checkscope(ClipMachineMemory, rd, ro, &sok, __PROC__)))
	 return er;
      if (!res || !sok)
      {
	 if ((er = ntx_next(ClipMachineMemory, rd, ro, __PROC__)))
	    return er;
      }
   }
   return 0;
}
