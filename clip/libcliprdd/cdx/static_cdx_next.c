static int
cdx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int out = 0, found;

   ClipVar v, *vp;

   void *key;

   int ok = 1, fok = 0, deleted, lastrec, er;

   rd->bof = rd->v_bof = 0;
   if (rd->eof)
      return 0;

   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
   {
      ro->valid_stack = 0;
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
      if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	 return er;
      rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      if (rd->filter->listlen == 0)
      {
	 rd->bof = rd->v_bof = 1;
	 rd->filter->cursor = 0;
      }
      return 0;
   }
   else
   {
      key = malloc(ro->bufsize);
      if (rd->shared || !ro->valid_stack)
      {
	 char cntcdx[4];

	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	    return er;
	 if (!ro->valid_stack || ro->cntcdx != _rdd_backuint((unsigned char *) cntcdx))
	 {
	    if (ro->custom)
	    {
	       memcpy(key, ro->key, ro->keysize);
	    }
	    else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
		  return er;
	       vp = _clip_vptr(&v);
	       if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
		  return er;
	       _clip_destroy(ClipMachineMemory, &v);
	    }
	    ro->curoffs = 0;

	    _CDX_HEADER(ro, &hdr);
	    ro->stack[0].page = _rdd_uint(hdr.root);
	    ro->level = 0;

	    if (!ro->descend)
	    {
	       if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
		  return er;
	    }
	    else
	    {
	       if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
		  return er;
	    }
	    //    _clip_destroy(ClipMachineMemory,&v);
	 }
      }

      while (ok || !fok)
      {
	 unsigned int recno;

	 if (!ro->descend)
	 {
	    if ((er = _cdx_next(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
	       return er;
	 }
	 else
	 {
	    if ((er = _cdx_prev(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
	       return er;
	 }
	 if (out)
	    break;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, recno, 1, __PROC__)))
	    return er;
	 if (!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG))
	    break;
	 if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	    return er;
	 if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	    return er;
	 if (ok > 0)
	 {
	    out = 1;
	    break;
	 }
	 if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	    return er;
      }
      free(key);
   }
   if (out)
   {
      unsigned int lastrec;

      if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) &lastrec, __PROC__)))
	 return er;
      rd->eof = 1;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	 return er;
      ro->valid_stack = 0;
   }
   else
   {
      ro->valid_stack = 1;
   }
   return 0;
}
