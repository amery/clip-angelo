static int
cdx_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int out, found;

   ClipVar v, *vp;

   void *key;

   int ok = 1, fok = 0, er;

   unsigned int oldrecno = rd->recno;

   unsigned int oldpage = ro->stack[ro->level].page;

   int oldpos = ro->stack[ro->level].pos;

   int lastrec, deleted;

   int oldeof = rd->eof;

   void *oldkey;

   if (rd->bof)
      return 0;
   if (rd->eof)
   {
      rd->eof = 0;
      if ((er = cdx_gobottom(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;
      rd->eof = rd->bof;
      return 0;
   }
   oldkey = malloc(ro->bufsize);
   memcpy(oldkey, ro->key, ro->keysize);
   key = malloc(ro->bufsize);
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
   {
      int oldcursor = rd->filter->cursor;

      while (rd->filter->cursor > 0)
      {
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[--rd->filter->cursor], 0, __PROC__)))
	    goto err1;
	 if (!(ClipMachineMemory->flags & DELETED_FLAG))
	    goto cont;
	 if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	    goto err1;
	 if (!deleted)
	    goto cont;
      }
      rd->bof = rd->v_bof = 1;
      if (rd->filter->listlen == 0)
      {
	 rd->eof = 1;
	 if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	    goto err1;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	    goto err1;
      }
      else
      {
	 rd->filter->cursor = oldcursor;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->filter->list[oldcursor], 0, __PROC__)))
	    goto err1;
      }
      goto cont;
   }
   if (rd->shared || !ro->valid_stack)
   {
      char cntcdx[4];

      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	 goto err1;
      if (!ro->valid_stack || ro->cntcdx != _rdd_backuint((unsigned char *) cntcdx))
      {
	 if (ro->custom)
	 {
	    memcpy(key, ro->key, ro->keysize);
	 }
	 else
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	       goto err1;
	    vp = _clip_vptr(&v);
	    if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	       goto err1;
	    _clip_destroy(ClipMachineMemory, &v);
	 }
	 ro->curoffs = 0;

	 _CDX_HEADER(ro, &hdr);
	 ro->stack[0].page = _rdd_uint(hdr.root);
	 ro->level = 0;

	 if (!ro->descend)
	 {
	    if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	       goto err1;
	 }
	 else
	 {
	    if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	       goto err1;
	 }
	 // _clip_destroy(ClipMachineMemory,&v);
      }
   }

   if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
      goto err1;
   if (!fok)
   {
      if ((er = cdx_next(ClipMachineMemory, rd, ro, __PROC__)))
	 goto err1;
      oldrecno = rd->recno;
      oldeof = rd->eof;
      fok = 1;
   }

   while (ok || !fok)
   {
      unsigned int recno;

      rd->eof = 0;
      if (!ro->descend)
      {
	 if ((er = _cdx_prev(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
	    goto err1;
      }
      else
      {
	 if ((er = _cdx_next(ClipMachineMemory, rd, ro, &out, &recno, __PROC__)))
	    goto err1;
      }
      if (out)
	 break;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, recno, 1, __PROC__)))
	 goto err1;
      if (!ro->scopetop && !ro->scopebottom && !rd->filter && !(ClipMachineMemory->flags & DELETED_FLAG))
	 break;
      if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, &rd->recno, key, __PROC__)))
	 goto err1;
      if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
	 goto err1;
      if (ok < 0)
      {
	 out = 1;
	 break;
      }
      if ((er = rdd_checkfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	 goto err1;
   }

   if (out)
   {
      rd->bof = rd->v_bof = 1;
      ro->stack[ro->level].page = oldpage;
      ro->stack[ro->level].pos = oldpos;
      if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__)))
	 goto err1;
      rd->eof = oldeof;
      memcpy(ro->key, oldkey, ro->keysize);
      ro->valid_stack = 0;
   }
   else
   {
      ro->valid_stack = 1;
   }
 cont:
   free(key);
   free(oldkey);
   return 0;
 err1:
   free(key);
   free(oldkey);
   return er;
}
