static int
cdx_ii_prev(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   CDX_HEADER hdr;

   int out, found, er;

   ro->level = 0;
   if (ro->bof)
      return 0;
   if (ro->eof)
   {
      ro->eof = 0;
      return 0;
   }
   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   if ((er = _cdx_search_tree(ClipMachineMemory, ro, ro->iikey, ro->bufsize, *(unsigned int *) ro->id, &found, &out, __PROC__)))
      return er;
   if (!found && out)
   {
      if ((er = cdx_ii_gotop(ClipMachineMemory, ro, __PROC__)))
	 return er;
      if ((er = cdx_ii_prev(ClipMachineMemory, ro, __PROC__)))
	 return er;
   }
   _CDX_PAGE(ro, ro->stack[ro->level].page, &page);
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_prev_rm(ClipMachineMemory, ro, leaf, &out, 0, __PROC__)))
      return er;
   if (!out)
   {
      if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
	 return er;
      if ((ro->type == 'C' || (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
	 loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
   }
   ro->bof = out;
   return 0;
}
