static int
_ntx_del_node(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   NTX_PAGE pp;

   int no, count, er;

   unsigned int page = ro->stack[ro->level].page;

   unsigned int child;

   NTX_BUCKET *bp;

   _ntx_page(ro, page, &pp);
   count = _rdd_ushort(pp.nkeys);
   no = ro->stack[ro->level].pos;
   bp = _ntx_buck(&pp, no);
   child = _rdd_uint(bp->left);

   if (child)
   {
      unsigned int cpage;

      int cno;

      NTX_BUCKET *cbp;

      NTX_PAGE cpp;

      unsigned int record;

      (ro->level)++;
      if (ro->level >= MAX_BTREE_DEEP)
	 return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_deepexceeded);
      ro->stack[ro->level].page = child;

      if ((er = _ntx_find_last(ClipMachineMemory, ro, __PROC__)))
	 return er;

      cpage = ro->stack[ro->level].page;
      cno = ro->stack[ro->level].pos;
      _ntx_page(ro, cpage, &cpp);
      cbp = _ntx_buck(&cpp, cno);
      record = _rdd_uint(cbp->recno);
      memcpy(bp->key, cbp->key, ro->keysize);
      _rdd_put_uint(bp->recno, record);
      if ((er = rdd_write(ClipMachineMemory, &ro->index->file, page, sizeof(NTX_PAGE), (char *) &pp, __PROC__)))
	 return er;

      if ((er = _ntx_remove_node(ClipMachineMemory, ro, cpage, &cpp, cno, __PROC__)))
	 return er;
   }
   else
   {
      if ((er = _ntx_remove_node(ClipMachineMemory, ro, page, &pp, no, __PROC__)))
	 return er;
   }
   if ((er = _ntx_balance(ClipMachineMemory, rd, ro, __PROC__)))
      return er;

   return 0;
}
