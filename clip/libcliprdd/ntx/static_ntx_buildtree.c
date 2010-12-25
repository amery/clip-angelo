static int
ntx_buildtree(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;

   int r, er;

   if (!rd->shared && !rd->eof)
   {
      _ntx_header(ro, &hdr);
      ro->stack[0].page = _rdd_uint(hdr.root);
      ro->level = 0;

      if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;

      if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
	 return er;
   }
   ro->valid_stack = 1;
   return 0;
}
