static int
ntx_delkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_HEADER hdr;

   int r, er;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
      return er;

   if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;

   ro->valid_stack = 0;
   if (r)
      return 0;

   if ((er = _ntx_del_node(ClipMachineMemory, rd, ro, __PROC__)))
      return er;
   ro->curoffs = 0;
   return 0;
}
