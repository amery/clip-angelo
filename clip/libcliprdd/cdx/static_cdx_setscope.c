static int
cdx_setscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * top, ClipVar * bottom, unsigned int *map, int bytes, int exact, const char *__PROC__)
{
   void *t = NULL;

   void *b = NULL;

   unsigned int firstrecno = 0, lastrecno = 0, recno;

   unsigned int lastrecno1 = -1;

   CDX_HEADER hdr;

   int found, out = 0;

   CDX_LEAF leaf;

   int bout, tout;

   int tlen = ro->bufsize;

   int blen = ro->bufsize;

   int er;

   if (top)
   {
      t = malloc(ro->bufsize);
      if ((er = cdx_formatkey(ClipMachineMemory, ro, top, t, __PROC__)))
	 goto err;
      if (top->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 if (exact)
	 {
	    tlen = ro->bufsize;
	 }
	 else
	 {
	    tlen = top->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	    if (ro->type == 'X')
	       tlen++;
	 }
      }
      else if (ro->type == 'X')
      {
	 blen = 9;
      }
   }
   if (bottom)
   {
      b = malloc(ro->bufsize);
      if ((er = cdx_formatkey(ClipMachineMemory, ro, bottom, b, __PROC__)))
	 goto err;
      if (bottom->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 if (exact)
	 {
	    blen = ro->bufsize;
	 }
	 else
	 {
	    blen = bottom->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	    if (ro->type == 'X')
	       blen++;
	 }
      }
      else if (ro->type == 'X')
      {
	 blen = 9;
      }
   }
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   if (b)
   {
      ro->level = 0;
      if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, b, blen, 0, &found, &bout, __PROC__)))
	 goto err;
      if (bout < 0)
	 goto e;
      if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &lastrecno, __PROC__)))
	 goto err;
      if (!found && !bout)
      {
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), &leaf, __PROC__)))
	    goto err;
	 if ((er = _cdx_next_rm(ClipMachineMemory, ro, &leaf, &bout, 0, __PROC__)))
	    goto err;
	 if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &lastrecno1, __PROC__)))
	    goto err;
      }
   }
   if (t)
   {
      ro->level = 0;
      if ((er = _cdx_search_tree(ClipMachineMemory, ro, t, tlen, 0, &found, &tout, __PROC__)))
	 goto err;
      if (tout > 0)
	 goto e;
      if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &firstrecno, __PROC__)))
	 goto err;
   }
   else
   {
      if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
	 goto err;
   }
   if (!bout && !tout && firstrecno == lastrecno1)
      goto e;
/* --------------------------- */
   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, ro->stack[ro->level].page, sizeof(CDX_LEAF), &leaf, __PROC__)))
      goto err;
   recno = -1;
   recno = *(unsigned int *) (leaf.keys + (ro->stack[ro->level].pos) * leaf.bytes) & _rdd_uint(leaf.recmask);

   while (!out && recno != lastrecno)
   {
      _rm_setbit(map, bytes, recno);
      if ((er = _cdx_next_rm(ClipMachineMemory, ro, &leaf, &out, 0, __PROC__)))
	 goto err;
      recno = *(unsigned int *) (leaf.keys + (ro->stack[ro->level].pos) * leaf.bytes) & _rdd_uint(leaf.recmask);
   }
   if (lastrecno && (recno == lastrecno))
      _rm_setbit(map, bytes, recno);
/* --------------------------- */
 e:
   ro->valid_stack = 0;
   if (t)
      free(t);
   if (b)
      free(b);
   return 0;
 err:
   ro->valid_stack = 0;
   if (t)
      free(t);
   if (b)
      free(b);
   return er;
}
