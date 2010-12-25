static int
cdx_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__)
{
   CDX_HEADER hdr;

   int found, ok, er;

   ClipVar v, *vp;

   void *key = malloc(ro->bufsize);

   unsigned int oldrecno = rd->recno;

   ro->level = 0;
   *keyno = 0;
   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
      return er;
   vp = _clip_vptr(&v);
   if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
      return er;
   _clip_destroy(ClipMachineMemory, &v);

   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

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
   if (!found)
      return 0;
   if ((er = _cdx_recno(ClipMachineMemory, ro, ro->level, &rd->recno, __PROC__)))
      return er;
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 1, __PROC__)))
      return er;
   if ((ro->type == 'C' || (ro->type == 'X' && ((char *) key)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_write(rd->loc, key, ro->bufsize);
   if ((er = _cdx_checkscope(ClipMachineMemory, rd, ro, key, &ok, __PROC__)))
      return er;
   free(key);
   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, oldrecno, 0, __PROC__)))
      return er;
   if (ok)
      return 0;
   if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, (unsigned int *) keyno, __PROC__)))
      return er;
   if (ro->scopetop)
   {
      unsigned int topno;

      ro->stack[0].page = _rdd_uint(hdr.root);
      ro->level = 0;
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
      if ((er = _cdx_keyno(ClipMachineMemory, ro, ro->stack[ro->level].page, ro->stack[ro->level].pos, (unsigned int *) &topno, __PROC__)))
	 return er;
      *keyno -= topno - 1;
   }
   return 0;
}
