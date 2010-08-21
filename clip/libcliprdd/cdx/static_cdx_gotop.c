static int
cdx_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   unsigned int lastrec;

   int out = 0, fok, deleted, er;

   ro->level = 0;
   rd->bof = rd->v_bof = rd->eof = 0;
   if (rd->filter && rd->filter->list)
   {
      if (rd->filter->listlen == 0)
      {
	 if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	    return er;
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 rd->filter->cursor = 0;
      }
      else
      {
	 rd->filter->cursor = -1;
	 while (++rd->filter->cursor < rd->filter->listlen)
	 {
	    if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[rd->filter->cursor], 0, __PROC__)))
	       return er;
	    if (!(ClipMachineMemory->flags & DELETED_FLAG))
	       return 0;
	    if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	       return er;
	    if (!deleted)
	       return 0;
	 }
	 rd->eof = rd->bof = rd->v_bof = 1;
	 if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	    return er;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
      return 0;
   }
   if (ro->scopetop)
   {
      int found;

      int ok = 0;

      CDX_HEADER hdr;

      void *key = malloc(ro->bufsize);

      int outrange;

      _CDX_HEADER(ro, &hdr);
      ro->stack[0].page = _rdd_uint(hdr.root);

      if (!ro->descend)
      {
	 if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, &outrange, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, ro->scopetop, ro->stoplen, 0, &found, &outrange, __PROC__)))
	    return er;
	 if (outrange > 0 && ro->stack[ro->level].pos > 0)
	 {
	    ro->stack[ro->level].pos--;
	    outrange = 0;
	 }
      }
      if (!outrange)
      {
	 if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	    return er;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	    return er;
	 if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	    return er;
      }
      free(key);
      if (outrange || ok)
      {
	 unsigned int lastrec;

	 if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	    return er;
	 rd->eof = 1;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    return er;
      }
   }
   else
   {
      if (!ro->descend)
      {
	 if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	    return er;
      }
      else
      {
	 if ((er = _cdx_last(ClipMachineMemory, ro, &out, __PROC__)))
	    return er;
      }
      if (out)
      {
	 rd->bof = rd->v_bof = rd->eof = 1;
      }
      else
      {
	 if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
	    return er;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
	    return er;
      }
   }
   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
      return er;
   if (!fok)
   {
      if ((er = cdx_next(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;
      if (rd->eof)
      {
	 rd->bof = rd->v_bof = 1;
	 return 0;
      }
   }
   return 0;
}
